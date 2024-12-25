#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>
#include <stdlib.h>
#include "task.h"
#include "stm32f401xc.h"

typedef struct semaphore
{
    volatile uint8_t count;
}Semaphore_Typedef;

uint8_t semaphore_wait(Semaphore_Typedef** semaphore, uint32_t timeout_ms);
void semaphore_signal(Semaphore_Typedef** semaphore);
Semaphore_Typedef* semaphore_create(uint8_t count);

#endif