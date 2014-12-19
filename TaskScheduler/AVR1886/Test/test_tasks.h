void Thread0(void);
void Thread1(void);
void Thread2(void);

#define  F_CPU 16000000UL
#include <util/delay.h>


int main(void)
{
	//unsigned char Thread1Stack[250];
	//unsigned char Thread2Stack[250];
	
	//for(int i = 0; i < 250;i ++)
	//{
	//	Thread1Stack[i] = 'C';
	//	Thread2Stack[i] = 'D';
	//}
	
	//_task_scheduler_add_task(Thread1, Thread1Stack, 250);
	//_task_scheduler_add_task(Thread2, Thread2Stack, 250);
	
	//DDRD |= (1 << 5);
	//PORTB |= (1 << 5);
	
	//TCCR1B =  (1 << CS00) | (1 << CS01) | (1 << CS02);
	
	//Thread0();
}

void Thread0(void)
{
	_delay_ms(500);
	while(1)
	{
		_delay_ms(500);
		PORTB &= ~(1 << 5);
	}
}

void Thread1(void)
{
	while(1)
	{
		_delay_ms(500);
		PORTB |= (1 << 5);
	}
}

void Thread2(void)
{
	while(1)
	{
		asm volatile("nop");
	}
}