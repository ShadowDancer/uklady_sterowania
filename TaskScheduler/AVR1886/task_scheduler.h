/*
 * task_scheduler_include.h
 *
 * Created: 2014-11-07 02:56:33	
 *  Author: Przemek
 */ 


#ifndef TASK_SCHEDULER_INCLUDE_H_
#define TASK_SCHEDULER_INCLUDE_H_

#define MAX_THREADS 2 // max number of tasks
#define __TASK_SCHEDULER_SCHEDULE TIMER1_COMPA_vect // overflow used by scheduler

#ifndef __ASSEMBLER__

	// functions exported to c module
	// magic, do not touch!

	// task scheduler variable definitions
	uint16_t __task_data[MAX_THREADS];
	uint8_t __task_current = 0;
	uint8_t __tasks_used = 1;
	
	extern void _task_scheduler_add_task (void (*entry_point)(void), void* stack_pointer, uint16_t stack_size);
	// add task to scheduler
	// make sure that stack won't be overwritten
	// library needs 33 bytes of stack
	
	extern void _task_scheduler_task_yeld(void);
	// yeld execution to next task in queue
	
	
	extern void _task_scheduler_task_exit(void);
	// task will be removed from scheduler
	// its memory may be reused
	// >>>WARNING<< task ids of tasks with higer ids will decrease by 1
	
#else
	// assembler variable definitions
	.extern task_data
	.extern __task_current
	.extern __tasks_used

#endif


#endif /* TASK_SCHEDULER_INCLUDE_H_ */