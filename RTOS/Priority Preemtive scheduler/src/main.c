#include "main.h"

extern task_list* task_pointer;

void task_1(void);                      // task function prototype
void task_2(void);
void task_3(void);
void Blink_task(void);

int main()
{
    SystemInit();                       // Initialize system

    Log_s("Priority Preemptive scheduler Program.\n");

    Log_s("Creating tasks...");
    createTask(task_1, "Task 1", 3);    // create task
    createTask(task_2, "Task 2", 2);
    createTask(task_3, "Task 3", 1);
    createTask(Blink_task, "Blink Task", 0);
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
        __disable_irq();
        Log_s("Running Task 1");
        __enable_irq();
        task_delay(500);
    }
}

void task_2(void)                       // this function toggles Port B pin 6
{
    while (1)
    {
        __disable_irq();
        Log_s("Running Task 2");
        __enable_irq();
        task_delay(500);
    }
}

void task_3(void)                       // this function toggles Port B pin 7
{
    while (1)
    {
        __disable_irq();
        Log_s("Running Task 3");
        __enable_irq();
        task_delay(1000);
    }
}

void Blink_task(void)
{
    while (1)
    {
        GPIOB->ODR ^= GPIO_ODR_OD5;
        task_delay(100);
    }
}