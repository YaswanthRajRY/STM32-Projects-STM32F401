#include "main.h"

void task_1(void);                      // task function prototype
void task_2(void);
void task_3(void);

int main()
{
    SystemInit();                       // Initialize system

    Log_s("Round Robin scheduler Program.\n");

    Log_s("Creating tasks...");
    createTask(task_1, "Task 1");       // create task
    createTask(task_2, "Task 2");
    createTask(task_3, "Task 3");
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
        GPIOB->BSRR |= GPIO_BSRR_BS5;
        for (uint32_t j = 0; j<1000000; j++);
        GPIOB->BSRR |= GPIO_BSRR_BR5;
        for (uint32_t j = 0; j<1000000; j++);
    }
}

void task_2(void)                       // this function toggles Port B pin 6
{
    while (1)
    {
        GPIOB->BSRR |= GPIO_BSRR_BS6;
        for (uint32_t j = 0; j<1000000; j++);
        GPIOB->BSRR |= GPIO_BSRR_BR6;
        for (uint32_t j = 0; j<1000000; j++);
    }
}

void task_3(void)                       // this function toggles Port B pin 7
{
    while (1)
    {
        GPIOB->BSRR |= GPIO_BSRR_BS7;
        for (uint32_t j = 0; j<1000000; j++);
        GPIOB->BSRR |= GPIO_BSRR_BR7;
        for (uint32_t j = 0; j<1000000; j++);
    }
}
