#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "stm32f401xc.h"

typedef struct
{
    TCB_Typedef* head;
}mutexList;

typedef struct mutex
{
    TCB_Typedef* owner;
    mutexList* waiting_task;
    volatile uint8_t lock;
}Mutex_Typedef;

Mutex_Typedef* createMutex(void);
void MutexTake(Mutex_Typedef** mutex);
void MutexGive(Mutex_Typedef** mutex);

#endif