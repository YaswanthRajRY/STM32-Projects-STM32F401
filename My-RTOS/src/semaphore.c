#include "semaphore.h"

extern uint32_t getSystemTime();                // define in scheduler.c

uint8_t semaphore_wait(Semaphore_Typedef** semaphore, uint32_t timeout_ms)
{
    uint32_t start = getSystemTime();
    while (1)
    {
        if ((*semaphore)->count > 0)
        {
            __disable_irq();
            (*semaphore)->count--;  // Acquire the semaphore
            __enable_irq();
            return 1;  // Successfully acquired
        }

        if ((getSystemTime() - start) >= timeout_ms)
        {
            return 0;  // Timeout occurred
        }
    }
    return 1;
}

void semaphore_signal(Semaphore_Typedef** semaphore)
{
    __disable_irq();

    (*semaphore)->count++;  // Increment semaphore count

    __enable_irq();
}

Semaphore_Typedef* semaphore_create(uint8_t count)
{
    Semaphore_Typedef* newSemaphore = (Semaphore_Typedef*)malloc(sizeof(Semaphore_Typedef));
    if (newSemaphore == NULL)
    {
        return NULL; // Handle memory allocation failure
    }

    newSemaphore->count = count;

    return newSemaphore;
}