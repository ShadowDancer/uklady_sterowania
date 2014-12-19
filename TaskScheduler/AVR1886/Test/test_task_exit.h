void Thread0(void);
void Thread1(void);

int main(void)
{
	unsigned char Thread1Stack[250];
	
	for(int i = 0; i < 250;i ++)
	{
		Thread1Stack[i] = 'C';
	}
	
	_task_scheduler_add_task(Thread1, Thread1Stack, 250);
	
	TCCR0B = 1;
	TIMSK0 = (1 << TOIE0);
	
	Thread0();
}

void Thread0(void)
{
	char MySREG = 0b11111111;
	SREG = MySREG;
	while(1)
	{
		_task_scheduler_task_exit();
	}
}

void Thread1(void)
{
	char MySREG = 0b11010100;
	SREG = MySREG;
	while(1)
	{
		asm volatile("nop");
	}
}
