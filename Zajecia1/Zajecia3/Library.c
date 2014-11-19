#include "Library.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static task_data tasks[MAX_NUMBER_OF_TASKS + 1];

void AddTask(int Priority, int Interval, void (*TaskPointer)(void* ptr), void* Params)
{
	tasks[Priority].TaskPointer = TaskPointer;
	tasks[Priority].Params = Params;
	tasks[Priority].Interval = Interval;
	tasks[Priority].Remaining = Interval;
	tasks[Priority].Ready = 0;
}

void RemoveTask(int Priority)
{
	tasks[Priority].TaskPointer = 0;
}


void execute()
{
	// tu gdzies trzeba wy³¹czyc przerwania, nie wiem czy dobrze
	while(1)
	{
		cli();
		for(int i = 0 ; i < MAX_NUMBER_OF_TASKS + 1; ++i)
		{
			if(tasks[i].Ready == 1)
			{
				tasks[i].Ready = 0;
				if(tasks[i].TaskPointer != 0)
				{
					sei();
					tasks[i].TaskPointer(tasks[i].Params);
					cli();
				}
				break;
			}
		}
		sei();
	}
}

void schedule()
{
	for(int i = 0 ; i < MAX_NUMBER_OF_TASKS + 1; ++i)
	{
		tasks[i].Remaining--;
		if(tasks[i].Remaining == 0)
		{
			tasks[i].Remaining = tasks[i].Interval;
			tasks[i].Ready = 1;
		}
		
	}
}
