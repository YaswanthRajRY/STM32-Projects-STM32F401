#include "main.h"

void task_1(void);                      // task function prototype
void task_2(void);
void task_3(void);
void blink(uint32_t pos);

Semaphore_Typedef* semaphore;

int main()
{
    SystemInit();                       // Initialize system
    
    semaphore = semaphore_create(1);

    Log_s("Priority Preemptive scheduler Program.\n");

    Log_s("Creating tasks...");
    createTask(task_1, "Task 1", 1);    // create task
    createTask(task_2, "Task 2", 2);
    createTask(task_3, "Task 3", 3);
    Log_s("Tasks created sucessfully.\n");

    Log_s("Starting scheduler...");
    start_scheduler();                  // Start scheduling the tasks

    while (1)
    {

    }
}

void task_1(void)                       // this function toggles Port B pin 5
{
    while (1)
    {
        if(semaphore_wait(&semaphore, 0))
        {
            blink(5);
            semaphore_signal(&semaphore);
        }
        task_delay(1000);
    }
}

void task_2(void)                       // this function toggles Port B pin 6
{
    while (1)
    {
        if(semaphore_wait(&semaphore, 0))
        {
            blink(6);
            semaphore_signal(&semaphore);
        }   
        task_delay(1000);
    }
}

void task_3(void)                       // this function toggles Port B pin 7
{
    while (1)
    {
        if (semaphore_wait(&semaphore, 0))
        {
            blink(7);
            semaphore_signal(&semaphore);
        }
        task_delay(1000);
    }
}

void blink(uint32_t pos)
{
    for (uint8_t i=0; i<6; i++)
    {
        GPIOB->ODR ^= (1 << pos);
        task_delay(100);
    }
}