#include "main.h"

void task_1(void);                      // task function prototype
void task_2(void);
void task_3(void);
void blink(uint32_t pos);

Mutex_Typedef* mutex;

int main()
{
    SystemInit();                       // Initialize system
    
    mutex = createMutex();

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
        blink(5);
        task_delay(1000);
    }
}

void task_2(void)                       // this function toggles Port B pin 6
{
    while (1)
    {
        blink(6);
        task_delay(1000);
    }
}

void task_3(void)                       // this function toggles Port B pin 7
{
    while (1)
    {
        GPIOB->ODR ^= GPIO_ODR_OD7;
        task_delay(500);
    }
}

void blink(uint32_t pos)
{
    while (1)
    {
        MutexTake(&mutex);
        for (uint8_t i=0; i<8; i++)
        {
            GPIOB->ODR ^= (1 << pos);
            task_delay(500);
        }
        MutexGive(&mutex);
    }
}