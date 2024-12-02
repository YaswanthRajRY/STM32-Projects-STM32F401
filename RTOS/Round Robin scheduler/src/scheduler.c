#include "scheduler.h"

extern task_list* task_pointer;			// defined in task.c
task_list* executing_task = NULL;		// points to executing task

TCB_Typedef* current_task = NULL;		// points to current task TCB

uint32_t stack_ptr;						// stores task stack address during context switching

/**************************************** Function to switch task ********************************************/
void switch_task()
{	
	current_task->stack_addr = (uint32_t*)stack_ptr;		// update psp in current task

	if (executing_task->next != NULL)
	{
		executing_task = executing_task->next;				// switch to next task
	}
	else
	{
		executing_task = task_pointer;						// start over from head
	}

	current_task = &executing_task->tcb;					// change current task pointer to scheduled nect task
	stack_ptr = (uint32_t)current_task->stack_addr;			// copy next stack address for restoring next task
}

/************************************ Function to do context switching ***************************************/
__attribute__((naked)) void PendSV_Handler(void)
{
	__ASM volatile("MRS r0, psp");								// move current psp to r0
	__ASM volatile("STMDB r0!, {r4-r11}");						// push r4-r11 in program stack
	__ASM volatile("MSR psp, r0");								// update psp
	__ASM volatile("PUSH {lr}");								// push lr in main stack
	__ASM volatile("MOV %0, r0" : "=r" (stack_ptr)::);			// copy updated psp to current tasl tcb
	
	__ASM volatile("BL switch_task");							// branch to switch function

	__ASM volatile("MOV r0, %0" :: "r" (stack_ptr):);			// copy next task stack address to r0
	__ASM volatile("LDMIA r0!, {r4-r11}");						// pop saved context from task stack to r4-r11
	__ASM volatile("MSR psp, r0");								// update psp
	__ASM volatile("POP {lr}");									// pop lr from main stack
	__ASM volatile("BX lr");									// return from exception
}

/************************************* Function for SysTick_Handler ******************************************/
void SysTick_Handler(void) 
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;  	// Set PendSV interrupt
}

/*********************************** Function to start scheduling task ***************************************/
void start_scheduler(void)
{
    SysTick_Config(42000000 / 1000);
    
    NVIC_SetPriority(PendSV_IRQn, 0xFF);

	executing_task = task_pointer;
	
	__set_PSP((uint32_t)executing_task->tcb.stack_addr);
	__set_CONTROL(0x2);
	__ISB();

	executing_task->tcb.fn();
}


