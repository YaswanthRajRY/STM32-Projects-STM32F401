#include "scheduler.h"

extern TCB_Typedef* idle_task;			// defined in task.c

extern ready_task_list* ReadyHead;
extern blocked_task_list* BlockedHead;

TCB_Typedef* next_task = NULL;		// used to iterate through circulat linked list

uint32_t stack_ptr;						// stores task stack address during context switching

/********************************** Function to get current running task *************************************/
TCB_Typedef* getCurrentTask()
{
	TCB_Typedef* currentTask;
    __ASM volatile("MOV %0, r12" : "=r" (currentTask));
    return currentTask;
}

/**************************************** Function to switch task ********************************************/
void switch_task()
{	
	TCB_Typedef* executing_task = getCurrentTask();
	executing_task->psp = (uint32_t*)stack_ptr;	// update executing task stack address for saving context
	//TCB_Typedef* temp = ReadyHead->head;
	//uint32_t prio = 255;
	
	if (ReadyHead->head == NULL)
	{
		executing_task = idle_task;
	}
	else
	{
		if (executing_task->next == NULL || next_task == NULL)
		{
			executing_task = ReadyHead->head;
			next_task = executing_task->next;
		}
		else
		{
			executing_task = next_task;
			next_task = executing_task->next;
		}
	}
	/*
	else
	{
		while (temp != NULL)
		{
			if (temp->priority < prio)
			{
				next_task = temp;
				prio = next_task->priority;
			}
			temp = temp->next;
		}

		if (next_task == NULL)
		{
			executing_task = idle_task;
		}
		else
		{
			executing_task = next_task;
		}
	}
	*/

	stack_ptr = (uint32_t)executing_task->psp;			// update next task stack address for restoring context
}

/************************************ Function to do context switching ***************************************/
void PendSV_Handler(void)
{
	__ASM volatile("MRS r0, psp");						// move current psp to r0
	__ASM volatile("STMDB r0!, {r4-r11}");					// push r4-r11 in program stack
	__ASM volatile("MSR psp, r0");						// update psp
	__ASM volatile("PUSH {lr}");						// push lr in main stack
	__ASM volatile("MOV %0, r0" : "=r" (stack_ptr)::);			// copy updated psp to current tasl tcb
	__ASM volatile("BL switch_task");					// branch to switch function
	__ASM volatile("MOV r0, %0" :: "r" (stack_ptr):);			// copy next task stack address to r0
	__ASM volatile("LDMIA r0!, {r4-r11}");					// pop saved context from task stack to r4-r11
	__ASM volatile("MSR psp, r0");						// update psp
	__ASM volatile("POP {lr}");						// pop lr from main stack
	__ASM volatile("BX lr");						// return from exception
}

/************************************ Function to update task ticks ******************************************/
static void update_task_delay()
{
	if (BlockedHead->head == NULL)
	{
		return;
	}
	
	TCB_Typedef* current_task = BlockedHead->head;
	TCB_Typedef* nexttask = NULL;
	
	while (current_task != NULL)
	{
		nexttask = current_task->next;

		if (current_task->delay == 0)
	    {
	        current_task->state = READY;				// if condition satisfies change task state to ready
			removeFromBlockedList(&current_task);
			addToReadyList(&current_task);
	    }
		else
		{
			current_task->delay--;
		}
	    current_task = nexttask;				// iterate to next task if condition not satisfies
	}
}

/********************************** Function to trigger PendSV interrupt *************************************/
static void set_PendSV()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; 					// Trigger PendSV for context switch
}

/**************************************** Function to dalay task *********************************************/
void task_delay(uint32_t ms)
{
	__disable_irq();							// disable interrupts

	TCB_Typedef* current_task = getCurrentTask();

	current_task->delay = ms;						// update task delay
	current_task->state = BLOCKED;						// change task state to blocked

	removeFromReadyList(&current_task);
	addToBlockedList(&current_task);

	set_PendSV();								// set PendSV interrupt

	__enable_irq();								// enable interrupts
}

/************************************* Function for SysTick_Handler ******************************************/
void SysTick_Handler(void) 
{
	update_task_delay();							// update task tick
    	set_PendSV();								// set PendSV to trigger interrupt
}

/****************************************** Idle task function ***********************************************/
void idleTask(void)
{
	SysTick_Config(42000000 / 1000);					// configure systick timer to interrupt every 1ms

	while (1);
}

/*********************************** Function to start scheduling task ***************************************/
void start_scheduler(void)
{
    NVIC_SetPriority(PendSV_IRQn, 0xFF);					// set PendSV IRQ in low priority

	create_idle_task();							// create idle task

	__ASM volatile("MOV r0, %0" :: "r" (idle_task->psp));	// move idle task stack address to r0
	__ASM volatile("LDMIA r0!, {r4-r11}");					// pop saved dummy context from task stack to r4-r11
	__ASM volatile("MOV r4, r0");
	__ASM volatile("LDMIA r4!, {r0-r3}");
	__ASM volatile("LDMIA r4!, {r12}");
	__ASM volatile("MSR psp, r4");					// update psp

	__ASM volatile("MOV r0, #2");
	__ASM volatile("MSR control, r0");				// set cpu in unprivileged mode						
	__ASM volatile ("isb 0xF":::"memory");				// instruction sync block

	idleTask();							// run idle task first
}
