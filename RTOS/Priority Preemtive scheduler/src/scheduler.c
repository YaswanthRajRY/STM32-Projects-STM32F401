#include "scheduler.h"

extern task_list* idle_task;			// defined in task.c
task_list* executing_task = NULL;		// points to executing task
task_list* task_iterator = NULL;		// used to iterate through circulat linked list

uint32_t stack_ptr;				// stores task stack address during context switching
uint32_t ticks = 0;				// track every 1ms tick

/********************************** Function to trigger PendSV interrupt *************************************/
static void set_PendSV()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; 						// Trigger PendSV for context switch
}

/**************************************** Function to switch task ********************************************/
void switch_task()
{	
	executing_task->tcb.stack_addr = (uint32_t*)stack_ptr;				// update executing task stack address for saving context

	task_list* scheduled_task = NULL;
	uint8_t highest_priority = 255;										// Initilize Lowest priority as initial
	
	do
	{
		task_iterator = task_iterator->next;

		if (task_iterator->tcb.task_state != READY)						// skip if task is not ready
		{
			continue;
		}

		if (task_iterator->tcb.task_priority < highest_priority && task_iterator != idle_task)	// select lowest priority task
		{
			scheduled_task = task_iterator;								// copy the task with highest priority in iteration
			highest_priority = scheduled_task->tcb.task_priority;		// update the highest current highest priority in each iteration
		}
	} while (task_iterator != idle_task);								// exit iteration if it reaches the start

	if (scheduled_task == NULL)											// schedule idle task if all task are not ready
	{
		executing_task = idle_task;
	}
	else
	{
		executing_task = scheduled_task;								// copy scheduled task to executing task
	}

	stack_ptr = (uint32_t)executing_task->tcb.stack_addr;				// update next task stack address for restoring context
}

/************************************ Function to do context switching ***************************************/
void PendSV_Handler(void)
{
	__ASM volatile("MRS r0, psp");							// move current psp to r0
	__ASM volatile("STMDB r0!, {r4-r11}");						// push r4-r11 in program stack
	__ASM volatile("MSR psp, r0");							// update psp
	__ASM volatile("PUSH {lr}");							// push lr in main stack
	__ASM volatile("MOV %0, r0" : "=r" (stack_ptr)::);				// copy updated psp to current tasl tcb
	__ASM volatile("BL switch_task");						// branch to switch function
	__ASM volatile("MOV r0, %0" :: "r" (stack_ptr):);				// copy next task stack address to r0
	__ASM volatile("LDMIA r0!, {r4-r11}");						// pop saved context from task stack to r4-r11
	__ASM volatile("MSR psp, r0");							// update psp
	__ASM volatile("POP {lr}");							// pop lr from main stack
	__ASM volatile("BX lr");							// return from exception
}

/**************************************** Function to dalay task *********************************************/
void task_delay(uint32_t ms)
{
	__disable_irq();								// disable interrupts
	executing_task->tcb.task_delay = ms + ticks;				        // update task delay
	executing_task->tcb.task_state = BLOCKED;					// change task state to blocked
	set_PendSV();									// set PendSV interrupt
	__enable_irq();									// enable interrupts
}

/************************************ Function to update task ticks ******************************************/
static void update_task_delay()
{
	task_iterator = idle_task;

	do
	{
	    if (task_iterator->tcb.task_state == BLOCKED && task_iterator->tcb.task_delay == ticks)
	    {
	        task_iterator->tcb.task_state = READY;					// if condition satisfies change task state to ready
	    }
	    task_iterator = task_iterator->next;					// iterate to next task if condition not satisfies
	} while (task_iterator != idle_task);						// end loop after one cycle
}

/************************************* Function for SysTick_Handler ******************************************/
void SysTick_Handler(void) 
{
	ticks++;									// update tick every 1ms
	update_task_delay();								// update task tick
    	set_PendSV();									// set PendSV to trigger interrupt
}

/****************************************** Idle task function ***********************************************/
void idleTask(void)
{
	SysTick_Config(42000000 / 1000);						// configure systick timer to interrupt every 1ms

	while (1);
}

/*********************************** Function to start scheduling task ***************************************/
void start_scheduler(void)
{
    NVIC_SetPriority(PendSV_IRQn, 0xFF);						// set PendSV IRQ in low priority

	create_idle_task();								// create idle task

	executing_task = idle_task;

	__set_PSP((uint32_t)executing_task->tcb.stack_addr);				// set psp of executing task stack address
	__set_CONTROL(0x2);								// set cpu in unprivileged mode
	__ISB();									// instruction sync block

	idleTask();									// run idle task first
}
