/*
 * Inzynierka_DiodyWS2812B.c
 *
 * Created: 24.07.2017 19:23:52
 * Author : User
 */ 
#define F_CPU 16000000UL // czestotliwosc taktowania ukladu wyswietlacza
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "LED_WS2812B.h"
#include "I2C.h"
#include "avr/interrupt.h"
#define cbi(port, bit) (port) &= ~(1 << (bit))
#define sbi(port, bit) (port) |= (1 << (bit))
#define opoznienie 250 // czas opoznienia w us
uint8_t tabP1[10][3]={
{0b11111111,0b10000000,0b11111111},		// tablie z zakodowanymi wartosciami od 0 do 9
{0b00000000,0b01000000,0b11111111},
{0b10001111,0b10000000,0b11110000},
{0b10000000,0b10000000,0b11111111},
{0b11110000,0b00000000,0b11111111},
{0b11110000,0b10000000,0b10001111},
{0b11111111,0b10000000,0b10001111},
{0b10000000,0b10000000,0b11111111},
{0b11111111,0b10000000,0b11111111},
{0b11110000,0b10000000,0b11111111}};
uint8_t tabP0[10][3]={
{0b11111111,0b00000001,0b11111111},
{0b00000000,0b10000000,0b11111111},
{0b00011111,0b00010001,0b11110001},
{0b00010001,0b00010001,0b11111111},
{0b11110000,0b00010000,0b11111111},
{0b11110001,0b00010001,0b00011111},
{0b11111111,0b00010001,0b00011111},
{0b00000000,0b00000000,0b11111111},
{0b11111111,0b00010001,0b11111111},
{0b11110001,0b00010001,0b11111111}};
uint8_t czas[6]; //zmienne przechowujace dane o czasie
uint8_t kolor=0; // zmienna wskazuj¹ca na dany kolor
uint8_t licznikPrzepelnien=0;
uint8_t kolorTmp; 
ISR(INT0_vect)
{
	for(int j=5;j>=0;j--)
	{
		if(j>3)
		{
			kolorTmp=(kolor+1)%6;
		}
		else if(j<=3 && j>1)
		{
			kolorTmp=(kolor+2)%6;
		}
		else if(j<=1 && j>=-1)
		{
			kolorTmp=(kolor+3)%6;
		}
		for(int l=2;l>=0;l--)
		{
			wyslij_dane_p1(tabP1[czas[j]][l], kolorTab[kolorTmp]); // realizacja kolejnych operacji dla diod
			czysc_p0(liczbaDiod);
			wyslij_reset_p1();
			wyslij_reset_p0();
			czysc_p1(liczbaDiod);
			_delay_us(opoznienie); // opóŸnienie zapewniaj¹ce wspó³liniowoœæ elementów w kolumnie
			wyslij_dane_p0(tabP0[czas[j]][l], kolorTab[kolorTmp]);
			wyslij_reset_p1();
			wyslij_reset_p0();
		}
		if(j==2||j==4)
		{
			wyslij_dane_p1(0b00100100, kolorTab[kolorTmp]);
			czysc_p0(liczbaDiod);
			wyslij_reset_p1();
			wyslij_reset_p0();
			czysc_p1(liczbaDiod);
			_delay_us(opoznienie);
			wyslij_dane_p0(0, kolorTab[kolorTmp]);
			wyslij_reset_p1();
			wyslij_reset_p0();
		}
		//else
		//{
			//_delay_us(150);
		//}
		//czysc_p0(liczbaDiod);
		//_delay_us(10);
		//wyslij_reset_p0();
	}
}
ISR(TIMER1_OVF_vect)
{
	licznikPrzepelnien++;
	if(licznikPrzepelnien==9)
	{
		licznikPrzepelnien=0;
		kolor++;
		if(kolor==5)
		{
			kolor=0;
		}
	}
}
void init()
{
	DDRD=(0<<PIND2);
	PORTD=(1<<PIND2);
	GICR=(1<<INT0); // inicjalizacja uk³adu przerwañ, portów mikrokontrolera i licznika.
	MCUCR=(1<<ISC01);
	TIMSK|=(1<<TOIE1);
	TCCR1B|=(1<<CS12)|(0<<CS10);
	sei();
	init_ws2812b();
	TWBR = 0x98; // inicjalizacja TWI
	ustaw_czas(16,36,20);
}	
int main(void)
{	
	init(); // inicjalizacja licznika, uk³adu przerwañ, licznika oraz I2C
    while (1) 
	{
		TWIStart();
		TWIWrite(0xA0);
		TWIWrite(0x02);
		TWIStart();
		TWIWrite(0xA1);
		sekundy=TWIReadACK(); // odczyt danych z PCF8583
		minuty=TWIReadACK();
		godziny=TWIReadNACK();
		TWIStop();
		czas[5]=sekundy & 0x0F;
		czas[4]=(sekundy& 0xF0) >> 4;
		czas[3]=minuty & 0x0F;
		czas[2]=(minuty& 0xF0) >> 4;  // zmiana wartoœci czasu z formatu BCD na format binarny, zapis informacji o jednoœciach i dziesi¹tkach czasu do osobnych elementów tablicy czas
		czas[1]=godziny & 0x0F;
		czas[0]=(godziny& 0xF0) >> 4;
	}
}