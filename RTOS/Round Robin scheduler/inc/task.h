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
typedef struct
{
    void (*fn)(void);
    uint32_t* stack_addr;
    char* taskName;
    uint32_t task_delay;
    uint16_t task_state;
    uint16_t task_priority;
}TCB_Typedef;

/*
    Task state.
*/
enum state
{
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED
};

/*
    This structure holds TCB and pointer to point next task
    in linked list
*/
typedef struct task
{
    TCB_Typedef tcb;
    struct task* next;
}task_list;

/*
    create task.
    func addr and task name is passed as argument
*/
void createTask(void (*fn)(void), char* name);
void create_idle_task();

#endif