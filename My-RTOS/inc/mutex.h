#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "stm32f401xc.h"

/*
    List to hold task waiting to aquire mutex
*/
typedef struct
{
    TCB_Typedef* head;
}mutexList;

/*
    Mutex
    contains task that holds mutex (owner), list of waiting task, lock
    and task priority that holds mutex  
*/
typedef struct mutex
{
    TCB_Typedef* owner;
    mutexList* waiting_task;
    volatile uint8_t lock;
    volatile uint8_t priority;
}Mutex_Typedef;

/*
    create mutex  
*/
Mutex_Typedef* createMutex(void);

/*
    task aquire mutex, if its already using by other task added to the mutex list  
*/
void MutexTake(Mutex_Typedef** mutex);

/*
   task leaving the mutex. next task waiting for the mutex will aquire the mutex  
*/
void MutexGive(Mutex_Typedef** mutex);

#endif