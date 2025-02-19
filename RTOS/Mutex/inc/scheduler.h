#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "stm32f401xc.h"
#include "Log.h"
#include "task.h"

/*
    start scheduling tasks
*/
void start_scheduler(void);
void task_delay(uint32_t ms);
TCB_Typedef* getCurrentTask(void);

#endif