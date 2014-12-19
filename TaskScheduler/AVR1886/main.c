#define F_CPU 16000000UL

// atmega 328
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "task_scheduler.h"

// funkcje dla w¹tków
void Thread0(void);
void Thread1(void);
void Thread2(void);

int main(void)
{
	// rezerwacja pamiêci na stosy tasków
	unsigned char Thread1Stack[100];
	unsigned char Thread2Stack[250];
		
	// dodanie tasków do task schedulera
	_task_scheduler_add_task(Thread1, Thread1Stack, 100);
	_task_scheduler_add_task(Thread2, Thread2Stack, 250);
	
	// IO
	DDRB |= (1 << 5);
	DDRB |= (1 << 0);
	DDRB |= (1 << 1);
	
	// segments
	DDRD |= 0xFF;
	PORTD = 0x0;
	
	// digits
	DDRC |= 0xFF;
	PORTC = 0x0;
	
	// przerwanie dla schedule
	TIMSK1 |= (1 << OCIE1A); // COMP overflow
	OCR1A  = 250; // 250 * 64 = 16MHz = 1000 przerwa? na sekund?
	TCCR1B |= (1 << CS00) | (1 << CS01) | (1 << WGM12);  // Prescaller 64
	
	// w¹tek g³ówny jest automatycznie rejestrowany jako task 0
	Thread0();
}


// digits to 8 liczbowe bufory przechowuj¹ce liczby do wyœwietlenia na wyœwietlaczu
volatile unsigned char digits[16] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	
// bufor w którym znajduj¹ siê liczby które aktualnie nale¿y wyœwietliæ (0 - pierszy, 8 - drugi)
volatile unsigned char usedBuffer = 8;

// obliczanie liczb pierwszych, producent
void Thread0(void)
{
	unsigned char currentBuffer = 0; // indeks w buforze do którego aktualnie piszemy
	unsigned char i; // licznik pêtli
	unsigned char modulo = 0;
	unsigned long currentNumber; // aktualnie sprawdzany numer
	unsigned long divisor; // dzielnik
	
	while(1)
	{
		// obliczanie liczb pierwszych
		for(currentNumber = 1000001; currentNumber < 100000000; currentNumber += 2) // pomijam liczby parzyste
		{
			// sprawdzanie dzielników
			for(divisor = 3; divisor <= currentNumber/2; divisor += 2)
			{
				modulo = (currentNumber % divisor) == 0;
				
				if(modulo)
				{
					goto NotPrime;
				}
			}
			
			// jeœli mamy liczbê pierwsz¹ to wpisujemy do bufora cyfry
			divisor = currentNumber;
			for(i = 0; i < 8; i++)
			{
				digits[currentBuffer + i] = divisor % 10;
				divisor = divisor / 10;
			}
			
			// zamieniamy wskaŸniki do buforów
			if(currentBuffer == 0)
			{
				usedBuffer = 0;
				currentBuffer = 8;
			}	
			else
			{	
				usedBuffer = 8;
				currentBuffer = 0;
			}
						
			// jeœli liczba nie jest pierwsza nie wypisujemy jej na wyœwietlaczu
			NotPrime:
			;
		}
	}
}


// miganie led
void Thread1(void)
{
	while(1)
	{
		// poczekaj na 1000 przerwañ
		for(int i = 0; i < 1000; i++)
		{
			_task_scheduler_task_yeld();
		}
		// xor led
		PORTB ^= (1 << 5);
	}
}

// tablica zawiera liczby, które nale¿y ustawiæ na porcie aby zaœwieciæ cyfry 0, 1, 2, 3, ...
char digitDefinitions[] = { 0b00111111,
	0b00000011,
	0b01101101,
	0b01100111,
	0b01010011,
	0b01110110,
	0b01111110,
	0b00100011,
	0b01111111,
	0b01110111 };


// prze³¹czanie kolejnych liczb na wyœwietlaczu
void Thread2(void) // task w ka¿dym przerwaniu wyœwietla inn¹ cyfrê, potem przekazuje kontrolê do producenta
{
	unsigned char currentDigit = 0; // aktualnie obs³ugiwany wyœwietlacz
	
	while(1)
	{			
		// pobierz dane dot. wyœwietlanej cyfry
		int currentDigitDef = ~digitDefinitions[digits[currentDigit + usedBuffer]];
		
		// wybierz aktualnie w³¹czon¹ cyfrê (pierwsze 6 port c, kolejne 2 port b)
		PORTB |= 3; 
		if(currentDigit <= 5) 
		{
			PORTC = ~(1 << currentDigit);
		}
		else
		{
			PORTC = 0xFF;
			PORTB &= ~(1 << (currentDigit - 6));
		}
		
		// w³¹cz odpowiedni¹ cyfrê
		PORTD = currentDigitDef;
		
		
		// kolejna cyfra
		currentDigit = (currentDigit + 1) % 8;
		
		// przeka¿ kontrlê do tasku licz¹cego
		_task_scheduler_task_yeld();
		
	}
}