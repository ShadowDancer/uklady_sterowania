#define F_CPU 16000000UL

// atmega 328
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "task_scheduler.h"

// funkcje dla w�tk�w
void Thread0(void);
void Thread1(void);
void Thread2(void);

int main(void)
{
	// rezerwacja pami�ci na stosy task�w
	unsigned char Thread1Stack[100];
	unsigned char Thread2Stack[250];
		
	// dodanie task�w do task schedulera
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
	
	// w�tek g��wny jest automatycznie rejestrowany jako task 0
	Thread0();
}


// digits to 8 liczbowe bufory przechowuj�ce liczby do wy�wietlenia na wy�wietlaczu
volatile unsigned char digits[16] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	
// bufor w kt�rym znajduj� si� liczby kt�re aktualnie nale�y wy�wietli� (0 - pierszy, 8 - drugi)
volatile unsigned char usedBuffer = 8;

// obliczanie liczb pierwszych, producent
void Thread0(void)
{
	unsigned char currentBuffer = 0; // indeks w buforze do kt�rego aktualnie piszemy
	unsigned char i; // licznik p�tli
	unsigned char modulo = 0;
	unsigned long currentNumber; // aktualnie sprawdzany numer
	unsigned long divisor; // dzielnik
	
	while(1)
	{
		// obliczanie liczb pierwszych
		for(currentNumber = 1000001; currentNumber < 100000000; currentNumber += 2) // pomijam liczby parzyste
		{
			// sprawdzanie dzielnik�w
			for(divisor = 3; divisor <= currentNumber/2; divisor += 2)
			{
				modulo = (currentNumber % divisor) == 0;
				
				if(modulo)
				{
					goto NotPrime;
				}
			}
			
			// je�li mamy liczb� pierwsz� to wpisujemy do bufora cyfry
			divisor = currentNumber;
			for(i = 0; i < 8; i++)
			{
				digits[currentBuffer + i] = divisor % 10;
				divisor = divisor / 10;
			}
			
			// zamieniamy wska�niki do bufor�w
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
						
			// je�li liczba nie jest pierwsza nie wypisujemy jej na wy�wietlaczu
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
		// poczekaj na 1000 przerwa�
		for(int i = 0; i < 1000; i++)
		{
			_task_scheduler_task_yeld();
		}
		// xor led
		PORTB ^= (1 << 5);
	}
}

// tablica zawiera liczby, kt�re nale�y ustawi� na porcie aby za�wieci� cyfry 0, 1, 2, 3, ...
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


// prze��czanie kolejnych liczb na wy�wietlaczu
void Thread2(void) // task w ka�dym przerwaniu wy�wietla inn� cyfr�, potem przekazuje kontrol� do producenta
{
	unsigned char currentDigit = 0; // aktualnie obs�ugiwany wy�wietlacz
	
	while(1)
	{			
		// pobierz dane dot. wy�wietlanej cyfry
		int currentDigitDef = ~digitDefinitions[digits[currentDigit + usedBuffer]];
		
		// wybierz aktualnie w��czon� cyfr� (pierwsze 6 port c, kolejne 2 port b)
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
		
		// w��cz odpowiedni� cyfr�
		PORTD = currentDigitDef;
		
		
		// kolejna cyfra
		currentDigit = (currentDigit + 1) % 8;
		
		// przeka� kontrl� do tasku licz�cego
		_task_scheduler_task_yeld();
		
	}
}