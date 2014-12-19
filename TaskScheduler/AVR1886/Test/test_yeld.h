void Thread0(void);
void Thread1(void);
void Thread2(void);
void Thread3(void);


int main(void)
{
	unsigned char Thread1Stack[250];
	unsigned char Thread2Stack[250];
	unsigned char Thread3Stack[250];
	
	for(int i = 0; i < 250;i ++)
	{
		Thread1Stack[i] = 'C';	
		Thread2Stack[i] = 'D';
		Thread3Stack[i] = 'E';
	}
	
	_task_scheduler_add_task(Thread1, Thread1Stack, 250);
	_task_scheduler_add_task(Thread2, Thread2Stack, 250);
	_task_scheduler_add_task(Thread3, Thread3Stack, 250);
	
	Thread0();
}

void Thread0(void)
{
	while(1)
	{
		_task_scheduler_task_yeld();
	}
}

void Thread1(void)
{
	while(1)
	{
		_task_scheduler_task_yeld();
	}
}

void Thread2(void)
{
	while(1)
	{
		_task_scheduler_task_yeld();
	}
}

void Thread3(void)
{
	while(1)
	{
		_task_scheduler_task_yeld();
	}
}