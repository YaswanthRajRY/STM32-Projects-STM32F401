#include "task.h"

ready_task_list ready_task_head = {NULL};
ready_task_list* ReadyHead = &ready_task_head;              // pointer to point head in ready linked list

blocked_task_list blocked_task_head = {NULL};
blocked_task_list* BlockedHead = &blocked_task_head;        // pointer to point head in blocked linked list

TCB_Typedef* idle_task = NULL;                              // pointer to point idle task in linked list

uint32_t task_stack[STACK_SIZE];                            // total task stack size
uint16_t current_stack_size = STACK_SIZE;                   // available task stack size

extern void idleTask(void);                                 // defined in scheduler.c

void removeFromReadyList(TCB_Typedef** tcb)
{
    TCB_Typedef *current = ReadyHead->head;
    TCB_Typedef *prev = NULL;

    while (current != NULL)
    {
        if (current == *tcb)
        {
            if (prev == NULL)
            {
                ReadyHead->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            current->next = NULL;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void removeFromBlockedList(TCB_Typedef** tcb)
{
    TCB_Typedef *current = BlockedHead->head;
    TCB_Typedef *prev = NULL;

    while (current != NULL)
    {
        if (current == *tcb)
        {
            if (prev == NULL)
            {
                BlockedHead->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            current->next = NULL;
            return;
        }
        prev = current;
        current = current->next;
    }
}

/******************************* Function to add blocked task in linked List *******************************/
void addToBlockedList(TCB_Typedef** tcb)
{
    (*tcb)->next = BlockedHead->head;
    BlockedHead->head = *tcb;
}

/*********************************** Function to Allocate task stack ***************************************/
static int allocate_task_stack(TCB_Typedef** tcb)
{
    if ((current_stack_size - 128) < 0)                 // check for available space
    {
        return -1;
    }
    current_stack_size = current_stack_size - 128;      // allocate stack space
    (*tcb)->psp = &task_stack[current_stack_size];         // update task stack top address

    return 0;
}

/*********************************** Function to Create dummy context **************************************/
static void create_context(TCB_Typedef** tcb)
{
    uint8_t i = 0;

    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0x01000000;            // xPSR
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)(*tcb)->fn | 1;        // (pc) fn address with LSB set for T-mode
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xFFFFFFFD;            // LR exception return code for thread mode
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)(*tcb);                // (r12) TCB address
 
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xDEADBABE;            // dummy reg value r0
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xDEADBABE;            // dummy reg value r1
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xDEADBABE;            // dummy reg value r2
    *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xDEADBABE;            // dummy reg value r3
    
    for (i=0; i<8; i++)
    {
        *(uint32_t*)(--(*tcb)->psp) = (uint32_t)0xDEADBABE;        // dummy reg value r4- r11
    }
}

/********************************* Function to add ready task in linked List *******************************/
void addToReadyList(TCB_Typedef** tcb)
{
    if (ReadyHead->head == NULL)
    {
        ReadyHead->head = *tcb;
        return;
    }

    if ((*tcb)->priority <= ReadyHead->head->priority)
    {
        (*tcb)->next = ReadyHead->head;
        ReadyHead->head = *tcb;
        return;
    }
    
    TCB_Typedef* temp = ReadyHead->head;
    while (temp->next != NULL && temp->next->priority < (*tcb)->priority)
    {
        temp = temp->next;
    }
    (*tcb)->next = temp->next;
    temp->next = *tcb;
    /*
   (*tcb)->next = ReadyHead->head;
   ReadyHead->head = *tcb;
    */
}

/*************************************** Function to create task *******************************************/
uint8_t createTask(void (*fn)(void), char* name, uint8_t task_priority)
{
    TCB_Typedef* newTask = (TCB_Typedef*)malloc(sizeof(TCB_Typedef));     // allocate memory

    if (newTask == NULL)
    {
        return -1;      // Failed to allocate memory
    }

    newTask->fn = fn;                       // add function address in tcb
    newTask->taskName = name;               // add name in tcb
    newTask->state = READY;                 // set task state
    newTask->priority = task_priority;      // add task priority
    newTask->delay = 0;                     // set delay as 0
    newTask->next = NULL;

    if (allocate_task_stack(&newTask) < 0)  // allocate stack for task
    {
        return -2;      // failed to allocate stack / stack full
    }

    create_context(&newTask);       // create dummy context

    addToReadyList(&newTask);        // add created task in linked list
    return 0;
}

uint8_t create_idle_task()
{
    TCB_Typedef* newTask = (TCB_Typedef*)malloc(sizeof(TCB_Typedef));       // allocate memory

    if (newTask == NULL)
    {
        return -1;      // Failed to allocate memory
    }

    newTask->fn = idleTask;                 // add function address in tcb
    newTask->taskName = "Idle Task";        // add name in tcb
    newTask->state = READY;                 // set task state
    newTask->priority = 0;                  // add task priority
    newTask->delay = 0;                     // set delay as 0
    newTask->next = NULL;

    if (allocate_task_stack(&newTask) < 0)      // allocate stack for task
    {
        return -2;      // failed to allocate stack / stack full
    }

    create_context(&newTask);       // create dummy context

    idle_task = newTask;            // add idle task before task head

    return 0;
}