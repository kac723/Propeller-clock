#define F_CPU 8000000UL //czêstotliwoœæ taktowania mikrokontrolera
#include <avr/io.h>
#include <avr/interrupt.h> //zaimportowanie bibliotek
#include <stdbool.h>
#include "util/delay.h"
#define Hz_poczatkowe 1 // pocz¹tkowa prêdkoœæ obrotowa
#define Hz_koncowe 13 // koñcowa prêdkoœæ obrotowa
#define Preskaler 8 // wartoœæ preskalera zastosowanego licznika
uint16_t OCR1A_koncowe= F_CPU/(6*Hz_koncowe*Preskaler*4); // przeliczenie koñcowej prêdkoœci obrotowej
uint16_t OCR1A_poczatkowe =F_CPU/(6*Hz_poczatkowe*Preskaler*4); // przeliczenie pocz¹tkowej prêdkoœci obrotowej 
/*TABLICA
	Nr fazy 1	2	3	4	5	6
	1A		1	1	x	0	0	x
	1EN		1	1	0	1	1	0
	2A		0	x	1	1	x	0
	2EN		1	0	1	1	0	1
	3A		x	0	0	x	1	1
	3EN		0	1	1	0	1	1
*/
uint8_t tab[6]={0b00001011,0b00100011,0b00101100,0b00001110,0b00110010,0b00111000}; // tablica okreœlaj¹ca pr¹dy cewek
//uint8_t tab uk³ad: 0b  00	   0  0	   1  0	   1  1
//						 xx	  3EN 3A  2EN 2A  1EN 1A
uint8_t i=0; // zmienna u¿yta do indeksowania kolejnych elementów tablicy tab
bool przyspieszaj=true; // zmianna okreœlaj¹ca czy silnik ma przyspieszaæ czy te¿ nie
ISR(TIMER1_COMPA_vect) 
{
		if(i==6)
		{
			i=0;	//w przypadku przekroczenia zakresu tablicy tab zmienna i jest ustawiana na 0
		}
		PORTC=tab[i]; // przekazanie aktualnej wartoœci tab[i] na PORTC w celu zmiany fazy silnika
		i++; //inkrementacja zmiennej i, czyli wskazanie na kolejn¹ fazê silnika
		if(przyspieszaj)
		{
			if(OCR1A>30000)
			{
				OCR1A=OCR1A-150; // dla wartosci OCR1A przyspieszanie nastêpuje znacz¹co poprzez zmniejszenie rejestru OCR1A o 150.
			}
			else if((OCR1A>15000) && (OCR1A <30000))
			{
				OCR1A=OCR1A-100; // zmniejszenie OCR1A o 100, spowolnienie przyspieszania
			}
			else if((OCR1A<15000) && (OCR1A>10000))
			{
				OCR1A=OCR1A-50; //zmniejszenie OCR1A o 50, spowolnienie przyspieszania
			}
			else if(OCR1A<10000)
			{
				OCR1A=OCR1A-1; // od wartoœci 10 000 OCR1A jest zmniejszane o 1. Zmniejszanie o wiêksz¹ wartoœæ mo¿e doprowadziæ do destabilizacji pracy silnika
			}
			if(OCR1A<OCR1A_koncowe)
			{
				przyspieszaj=false; // silnik osi¹gniê prêdkoœæ koñcow¹, jeœli wartoœæi OCR1A jest mniejsza od OCR1_koncowe. Nastêpuje wtedy zakonczenie przyspieszania
			}
		}
}
int main(void)
{
	DDRC=0xff;
	PORTC=0xff; //ustawienie portu C mikrokontrolera jako wyjœæie
	OCR1A=OCR1A_poczatkowe; // inicjalizacja wartoœæi OCR1A
	TCCR1B|=(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10); // konfiguracja licznika mikrokontrolera na przerawanie poprzez porównanie
	TIMSK|=(1<<OCIE1A); // w³aczenie przerwania poprzez porównanie
	sei(); //w³aczenie przerwañ w mikrokontrolerze
    while (1) 
    {
		//petla nieskonczona, obs³uga silnika odbywa siê w ramach obs³ugi przerwania licznika
	}
}