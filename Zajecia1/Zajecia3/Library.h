#ifndef _LIBRARY_H_

#define _LIBRARY_H_

#define MAX_NUMBER_OF_TASKS 3

// dane taska
typedef struct
{
	void (*TaskPointer)(void* ptr);	// funkcja
	void* Params; // parametry
	int Interval; // co ile kwant�w ma by� wywyo�ywana funkcja
	int Remaining; // ile pozosta�o
	char Ready; // czy gotowe do wykonania
} task_data;

// dodaje taska do schedulera
void AddTask(int Priority, int Interval, void (*TaskPointer)(void* ptr), void* Params);

// usuwa task z schedulera
void RemoveTask(int Priority);

// p�tla
void execute();

// funkcja wywo�ywana co kwant czasu
void schedule();



#endif