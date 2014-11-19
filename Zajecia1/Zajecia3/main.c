#include <avr/io.h>
#include <avr/interrupt.h>

#include "Library.h"

// kod do testowania bibiloteki

void Led1(void* Params)
{
	int y = 0;
	y = 6;
}

void Led2(void* Params)
{
	int x = 0;
	x = 5;
}


void Led3(void* Params)
{
	int z = 0;
	z = 5;
}

ISR(TIMER0_COMP_vect)
{
	schedule();
}


int main(void)
{
	// Timer 0 w trybie OCR
	TIMSK |= (1 << OCIE0); // COMP overflow
	OCR0  = 250; // 250 * 64 = 16MHz = 1000 przerwañ na sekundê
	TCCR0 |= (1 << CS00) | (1 << CS01);  // Prescaller 64
	sei();
	
	
	AddTask(0, 1, &Led1, 0);
	AddTask(1, 2, &Led2, 0);
	AddTask(2, 3, &Led3, 0);
	
	execute();
}
