#include <avr/io.h>
#include "task_scheduler.h"

; ************ TASK ADD ************
.global _task_scheduler_add_task
_task_scheduler_add_task: ; setup variables and stack for task
; parametrs
; adress of entry point (R25-24)
; adress of memory for stack (R23-22) (R22 - low byte, R23 - high byte)
; size of memory for stack (R21-R20)
; used registers: X, Z, R16

CLI

; add stack size to stack adress and substract 1
; (so we have end of memory location in register)
	ADD R22, R20
	ADC R23, R21

	SUBI R22, 1
	SBC R23, R1
	
; registers R22:23 hold now stack adress, R20-21 are free
	

; save current stack to R20:21 register
	IN R20, _SFR_IO_ADDR(SPL)
	IN R21, _SFR_IO_ADDR(SPH)

; load new stack pointer
	OUT _SFR_IO_ADDR(SPL), R22
	OUT _SFR_IO_ADDR(SPH), R23

; push entry point to new stack
	PUSH R24
	PUSH R25
	
; save register default values to R1
	LDI R27, 0xAC

; push 31 registers to new stack
	PUSH R27
	PUSH R1
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	PUSH R27
	
	; push sreg on stack
	LDI R27, 0 ; load empty status register
	PUSH R27

; load place to store stack pointer
	LDS R26, __tasks_used ; R26 - new task id

; load adress of array with task stack pointers
	LDI ZL, lo8(__task_data)
	LDI ZH, hi8(__task_data)

; move to first free field in that array
MEMORY_MOVE_LOOP:
	CP R26, R1 ; if 0 remaining continue
	BREQ MEMORY_MOVE_AFTER
	ADIW ZL, 2
	DEC R26
	RJMP MEMORY_MOVE_LOOP	
MEMORY_MOVE_AFTER:
	
; save new stack pointer to array
	IN R22, _SFR_IO_ADDR(SPL)
	IN R23, _SFR_IO_ADDR(SPH)
	ST Z+, R22
	ST Z, R23

; restore stack from R20:21 register
	OUT _SFR_IO_ADDR(SPL), R20
	OUT _SFR_IO_ADDR(SPH), R21

; increment tasks used
	LDS R26, __tasks_used
	INC R26
	STS __tasks_used, R26

; return
	RETI

; ************ END OF TASK ADD ************

; ************ SCHEDULE ************
.global __TASK_SCHEDULER_SCHEDULE
__TASK_SCHEDULER_SCHEDULE:
	; save registers to task stack
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	PUSH R10
	PUSH R11
	PUSH R12
	PUSH R13
	PUSH R14
	PUSH R15
	PUSH R16
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	PUSH R21
	PUSH R22
	PUSH R23
	PUSH R24
	PUSH R25
	PUSH R26
	PUSH R27
	PUSH R28
	PUSH R29
	PUSH R30
	PUSH R31
	
	; push sreg on stack
	IN R16, _SFR_IO_ADDR(SREG)
	PUSH R16

	; copy program counter from temp to scheduled task
	LDS R16, __task_current

	; save stack pointer
	; load stack pointer array to z
	LDI ZL, lo8(__task_data)
	LDI ZH, hi8(__task_data)

	CLR R1
	
	; move z to desired array cell
	FIND_TASK_DATA_LOOP:
		CP R16, R1 ; if R16 == 0 continue
		BREQ FIND_TASK_DATA_LOOP_AFTER
		ADIW ZL, 2
		DEC R16
		RJMP FIND_TASK_DATA_LOOP
	FIND_TASK_DATA_LOOP_AFTER:
	
	; save stack pointer in array
	IN R16, _SFR_IO_ADDR(SPL)
	ST Z+, R16
	IN R16, _SFR_IO_ADDR(SPH)
	ST Z, R16

	; load next task
	LDS R16, __task_current
	INC R16
	LDS R17, __tasks_used
	CP R16, R17
	BRNE SCHEDULE_AFTER_INCRMENT_CURRENT_TASK
	LDI R16, 0 ; move back to beginning of task array
	
	SCHEDULE_AFTER_INCRMENT_CURRENT_TASK:
	; save new current task number
	STS __task_current, R16 ; keep value in R16

	SCHEDULE_FROM_TASK_EXIT:

	; find next task
	LDI ZL, lo8(__task_data)
	LDI ZH, hi8(__task_data)

	; move data in z register to 
	FIND_TASK_DATA_LOOP_2:
		CP R16, R1 ; if R16 == 0 continue
		BREQ FIND_TASK_DATA_LOOP_AFTER_2
		ADIW ZL, 2
		DEC R16
		RJMP FIND_TASK_DATA_LOOP_2
	FIND_TASK_DATA_LOOP_AFTER_2:
	
	; load stack pointer
	LD R16, Z+
	OUT	_SFR_IO_ADDR(SPL), R16
	LD R16, Z
	OUT	_SFR_IO_ADDR(SPH), R16

	
	; pop sreg
	POP R31
	OUT _SFR_IO_ADDR(SREG), R31

	; load registers
	POP R31
	POP R30
	POP R29
	POP R28
	POP R27
	POP R26
	POP R25
	POP R24
	POP R23
	POP R22
	POP R21
	POP R20
	POP R19
	POP R18
	POP R17
	POP R16
	POP R15
	POP R14
	POP R13
	POP R12
	POP R11
	POP R10
	POP R9
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0

	; proceed to task
	RETI

; ************ END OF SCHEDULE ************

; ************ YELD ************
.global _task_scheduler_task_yeld
_task_scheduler_task_yeld:
	; yeld execution to next task in queue

	; disable interrupts
	CLI

	; jump to next task
	RJMP __TASK_SCHEDULER_SCHEDULE
	
; ************ END OF YELD ************

; ************ EXIT ************
.global _task_scheduler_task_exit
_task_scheduler_task_exit:
	; disable interrupts
	CLI
	
	; hold number of remaining tasks
	LDS R18, __tasks_used
	LDS R24, __task_current
	
	; if there is only 1 task perform soft reset
	CP R18, R1
	BRNE EXIT_CONTINUE ; 
		JMP 0x00
	EXIT_CONTINUE:

	; decrement number of active tasks
	MOV R19, R18
	DEC R19
	STS __tasks_used, R19

	; find target task
	LDI ZL, lo8(__task_data)
	LDI ZH, hi8(__task_data)
	; keep adress of next register (so we can copy it)
	LDI XL, lo8(__task_data + 2)
	LDI XH, hi8(__task_data + 2)

	; move to task to kill
	KILL_TASK_FIND_TASK_TO_KILL_LOOP:
		CP R24, R1 ;
		BREQ KILL_TASK_FIND_TASK_TO_KILL_AFTER
		ADIW ZL, 2
		ADIW XL, 2
		DEC R24
		DEC R18
		RJMP KILL_TASK_FIND_TASK_TO_KILL_LOOP
	KILL_TASK_FIND_TASK_TO_KILL_AFTER:

	; move stack pointer of other tasks back
	DEC R18 ; amount of remaining registers to move
	KILL_TASK_MOVE_ADRESSES:
		CP R18, R1 ; if more than 0 adresses to copy left
		BREQ KILL_TASK_FINISHED
		LD R19, X+
		ST Z+, R19
		LD R19, X+
		ST Z+, R19
		DEC R18
		RJMP KILL_TASK_MOVE_ADRESSES

	; decrement current stack id, so we'll execute
	KILL_TASK_FINISHED:

	MOV R16, R24
	RJMP SCHEDULE_FROM_TASK_EXIT

	; return
	RETI

; ************ END OF EXIT *************