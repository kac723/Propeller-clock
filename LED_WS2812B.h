#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdbool.h>
#define cbi(port, bit) (port) &= ~(1 << (bit))
#define sbi(port, bit) (port) |= (1 << (bit))
#define LEDPORTD DDRD
#define LEDPORT PORTD
#define LEDPINfirst 1
#define LEDPINsecond 0
#define liczbaDiod 8
uint32_t kolorTab[6]={0xff0000,0xffff00,0xff00ff,0x00ff00,0x0000ff,0x00ffff}; // zakodowane kolory: czerwony, ��ty, fioletowy, zielony, niebieski, b��kitny 
void init_ws2812b()
{
	LEDPORTD|=(1<<LEDPINfirst)|(1<<LEDPINsecond);
	LEDPORT|=(1<<LEDPINsecond)|(1<<LEDPINsecond);
	
}
void wyslij_jeden_p0()
{
	sbi(LEDPORT,LEDPINfirst); // ustawienie bitu LEDPINfirst na porcie LEDPORT
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	cbi(LEDPORT,LEDPINfirst); // ustawieni bitu LEDPINfirst na 0
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
}
void wyslij_zero_p0()
{
	sbi(LEDPORT,LEDPINfirst); // ustawienie bitu LEDPINfirst na porcie LEDPORT
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	cbi(LEDPORT,LEDPINfirst); // ustawieni bitu LEDPINfirst na 0
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
}
void wyslij_reset_p0()
{
	cbi(LEDPORT,LEDPINfirst); // ustawieni bitu LEDPINfirst na 0
	for(int i=0;i<110;i++)
	{
		asm  ("nop");
	}
	
}
void wyslij_dane_p0(uint8_t diodmask,uint32_t kolormask)
{
	uint32_t kolor; // zmienna pomocnicza dla koloru diod
	uint16_t G = (kolormask >> 8)& 0xFF00;
	uint32_t R = kolormask << 8 & 0xFF0000; // zamiana koloru z RGB na format GRB
	uint8_t B = kolormask & 0xff ;
	kolormask = G | R | B; // zapami�tanie koloru w formacie GRB
	bool dioda; // zmienna okre�laj�ca czy dana dioda ma �wieci� czy nie
	for (int j = 0; j<8; j++)
	{
		dioda = diodmask & 0x01;
		if (dioda) // obs�uga diod �wiec�cych
		{
			kolor = kolormask; // zapis zapami�tanej warto�ci kolormask do zmiennej kolor
			for (int i = 0; i<24; i++)
			{
				if (kolor & 0x800000) // sprawdzenie czy aktualny bit warto�ci kolor to 1
				{
					wyslij_jeden_p0();
					//je�li aktualny bit warto�ci kolor jest r�wny jeden nale�y przes�a� bit 1
				}
				else
				{
					wyslij_zero_p0();
					//je�li aktualny bit warto�ci kolor jest r�wny zero nale�y przes�a� bit 0
				}
				kolor = kolor << 1; // przesuni�cie warto�ci kolor w celu pobrania kolejnego bitu koloru
			}
		}
		else
		{
			for (uint8_t k = 0; k<24; k++) // do diod nieobs�ugiwanych 	wysy�ane s� same zera
			{
				wyslij_zero_p0();
			}
		}
		diodmask = diodmask >> 1; // przesuni�cie maski diod
	}
}

void czysc_p0(uint8_t diod_N)
{
	for(int j=0;j<diod_N;j++)
	{
		for(int i=0;i<24;i++)
		{
			wyslij_zero_p0();
		}
	}
}

void wyslij_jeden_p1()
{
	sbi(LEDPORT,LEDPINsecond);  // ustawienie bitu LEDPINsecond na porcie LEDPORT
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	cbi(LEDPORT,LEDPINsecond); // ustawieni bitu LEDPINsecond na 0
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
}
void wyslij_zero_p1()
{
	sbi(LEDPORT,LEDPINsecond);  // ustawienie bitu LEDPINsecond na porcie LEDPORT
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	cbi(LEDPORT,LEDPINsecond); // ustawieni bitu LEDPINsecond na 0
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
	asm  ("nop");
}
void wyslij_reset_p1()
{
	cbi(LEDPORT,LEDPINsecond); // ustawieni bitu LEDPINsecond na 0
	for(int i=0;i<110;i++)
	{
		asm  ("nop");
	}
	
}
void wyslij_dane_p1(uint8_t diodmask,uint32_t kolormask)
{
	uint32_t kolor;
	uint16_t R = (kolormask >> 8)& 0xFF00;
	uint32_t G = kolormask << 8 & 0xFF0000;
	uint8_t B = kolormask & 0xff ;
	kolormask = R | G | B;
	bool dioda;
	for (int j = 0; j<8; j++)
	{
		dioda = diodmask & 0x01;
		if (dioda)
		{
			kolor = kolormask;
			for (int i = 0; i<24; i++)
			{
				if (kolor & 0x800000)
				{
					wyslij_jeden_p1();
				}
				else
				{
					wyslij_zero_p1();
				}
				kolor = kolor << 1;
			}
		}
		else
		{
			for (uint8_t k = 0; k<24; k++)
			{
				wyslij_zero_p1();
			}
		}
		diodmask = diodmask >> 1;
	}
}
void czysc_p1(uint8_t diod_N)
{
	for(int j=0;j<diod_N;j++)
	{
		for(int i=0;i<24;i++)
		{
			wyslij_zero_p1();
		}
	}
}