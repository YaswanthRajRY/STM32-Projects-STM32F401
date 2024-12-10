#include "task.h"

task_list* task_head = NULL;                // pointer to point head in linked list
task_list* idle_task = NULL;                // pointer to point idle task in linked list

uint32_t task_stack[STACK_SIZE];            // total task stack size
uint16_t current_stack_size = STACK_SIZE;   // available task stack size

extern void idleTask(void);                 // defined in scheduler.c

/*********************************** Function to Allocate task stack ***************************************/
static int allocate_task_stack(TCB_Typedef* tcb, void (*fn)(void), char* name)
{
    if ((current_stack_size - 128) < 0)                             // check for available space
    {
        return -1;
    }
    current_stack_size = current_stack_size - 128;                  // allocate stack space
    tcb->stack_addr = &task_stack[current_stack_size];              // update task stack top address

    return 0;
}

/*********************************** Function to Create dummy context **************************************/
static void create_context(TCB_Typedef* tcb)
{
    uint8_t i = 0;

    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0x01000000;         // xPSR
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)tcb->fn | 1;        // (pc) fn address with LSB set for T-mode
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xFFFFFFFD;         // LR exception return code for thread mode
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)tcb;                // (r12) TCB address
    
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xDEADBABE;         // dummy reg value r0
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xDEADBABE;         // dummy reg value r1
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xDEADBABE;         // dummy reg value r2
    *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xDEADBABE;         // dummy reg value r3
    
    for (i=0; i<8; i++)
    {
        *(uint32_t*)(--tcb->stack_addr) = (uint32_t)0xDEADBABE;    // dummy reg value r4- r11
    }
}

/****************************** Function to Add task in linked List *******************************/
static void add_inList(task_list* tcb)
{
    if (task_head == NULL)                                          // check head is empty
    {
        task_head = tcb;
        return;
    }
    
    if (tcb->tcb.task_priority <= task_head->tcb.task_priority)     // add new high priority node in front
    {
        tcb->next = task_head;
        task_head = tcb;
        idle_task->next = task_head;                                // alwas idle task is at first before task head
        return;
    }

    task_list* current = task_head;

    while (current->next != NULL && current->next->tcb.task_priority <= tcb->tcb.task_priority)     // add new node in ascending order.
    {
        current = current->next;
    }
    tcb->next = current->next;
    current->next = tcb;
    return;
}

/*************************************** Function to create task *******************************************/
void createTask(void (*fn)(void), char* name, uint8_t task_priority)
{
    task_list* newTask = (task_list*)malloc(sizeof(task_list));     // allocate memory

    if (newTask == NULL)
    {
        Log_s("- Failed to create memory for task");
    }
    newTask->next = NULL;

    memset(newTask, 0, sizeof(*newTask));                           // set 0 as initial value

    if (allocate_task_stack(&newTask->tcb, fn, name) < 0)           // allocate stack for task
    {
        Log_s("- Stack full");
    }

    newTask->tcb.fn = fn;                                           // add function address in tcb
    newTask->tcb.taskName = name;                                   // add name in tcb
    newTask->tcb.task_state = READY;                                // set task state
    newTask->tcb.task_priority = task_priority;                     // add task priority

    create_context(&newTask->tcb);                                  // create dummy context

    add_inList(newTask);                                            // add created task in linked list
}

void create_idle_task()
{
    task_list* newTask = (task_list*)malloc(sizeof(task_list));     // allocate memory

    if (newTask == NULL)
    {
        Log_s("- Failed to create memory for task");
    }
    newTask->next = NULL;

    memset(newTask, 0, sizeof(*newTask));                           // set 0 as initial value

    if (allocate_task_stack(&newTask->tcb, idleTask, "Idle Task") < 0)           // allocate stack for task
    {
        Log_s("- Stack full");
    }

    newTask->tcb.fn = idleTask;                                     // add function address in tcb
    newTask->tcb.taskName = "Idle Task";                            // add name in tcb
    newTask->tcb.task_state = READY;                                // set task state
    newTask->tcb.task_priority = 0;                                 // add task priority

    create_context(&newTask->tcb);                                  // create dummy context

    newTask->next = task_head;
    idle_task = newTask;                                            // add idle task before task head
}