/*
 * DivisionTest.c
 *
 * Created: 2014-12-01 11:15:14
 *  Author: Przemek
 */ 


#include <avr/io.h>

int main(void)
{
    while(1)
    {
		volatile uint32_t number1 = 93420234ul;
		volatile uint32_t number2 = 23443242ul;
		
		
		
		uint32_t result = number1 / number2;
		
		TCCR1B = result;
		
    }
}