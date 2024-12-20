#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f401xc.h"
#include "Log.h"

/*
    total stack size. modify the size of user needs
*/
#define STACK_SIZE 1024

/*
    Task control block.
    contains func addr, stack addr and task name.
*/
typedef struct TaskControlBlock
{
    struct TaskControlBlock* next;
    void (*fn)(void);
    char* taskName;
    uint32_t* psp;
    uint32_t delay;
    uint16_t state;
    uint16_t priority;
}TCB_Typedef;

/*
    List to hold ready state tasks
*/
typedef struct ready
{
    TCB_Typedef* head;
}ready_task_list;

/*
    List to hold blocked state tasks
*/
typedef struct blocked
{
    TCB_Typedef* head;
}blocked_task_list;

/*
    Task state.
*/
enum state
{
    RUNNING,
    READY,
    WAITING,
    BLOCKED,
    SUSPENDED
};

/*
    create task.
    func addr and task name is passed as argument
*/
uint8_t createTask(void (*fn)(void), char* name, uint8_t task_priority);
uint8_t create_idle_task(void);
void addToReadyList(TCB_Typedef** tcb);
void addToBlockedList(TCB_Typedef** tcb);
void removeFromReadyList(TCB_Typedef** tcb);
void removeFromBlockedList(TCB_Typedef** tcb);

#endif