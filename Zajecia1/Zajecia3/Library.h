#ifndef _LIBRARY_H_

#define _LIBRARY_H_

#define MAX_NUMBER_OF_TASKS 3

typedef struct
{
	void (*TaskPointer)(void* ptr);	
	void* Params;
	int Interval;
	int Remaining;
	char Ready;
} task_data;

void AddTask(int Priority, int Interval, void (*TaskPointer)(void* ptr), void* Params);

void RemoveTask(int Priority);


void execute();

void schedule();



#endif