#include "Library.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// tablica przechowujπca dane o taksach
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
	while(1)
	{
		// w≥πcz przerwania w przypadku gdy øaden task nie jest gotowy
		cli();
		// sprawdü czy jest gotowy task
		for(int i = 0 ; i < MAX_NUMBER_OF_TASKS + 1; ++i)
		{
			if(tasks[i].Ready == 1)
			{
				// jeúli tak, to zresetuj timer
				tasks[i].Ready = 0;
				if(tasks[i].TaskPointer != 0)
				{
					// wykonaj task
					sei(); // w≥πcz przerwania, aby wywo≥aÊ ew. schedule podczas wykonywania taska
					tasks[i].TaskPointer(tasks[i].Params); // wykonaj zadanie
					cli();
				}
				break; // jeúli wykonano task zacznij przeglπdaÊ kolejkÍ od najwyøszego priorytetu
			}
		}
		sei();
	}
}

void schedule()
{
	// przeglπdnij tablicÍ taskÛw
	for(int i = 0 ; i < MAX_NUMBER_OF_TASKS + 1; ++i)
	{
		// zmniejsz czasy
		tasks[i].Remaining--;
		if(tasks[i].Remaining == 0) // jesli task zosta≥ dodany
		{
			// oznacz jako gotowy
			tasks[i].Remaining = tasks[i].Interval;
			tasks[i].Ready = 1;
		}
		
	}
}
