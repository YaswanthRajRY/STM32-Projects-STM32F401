#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "stm32f401xc.h"
#include "Log.h"
#include "task.h"

/*
    start scheduling tasks
*/
void start_scheduler(void);

/*
    this function puts the task in waiting state for a ms period of time   
*/
void task_delay(uint32_t ms);

/*
    function to get current running task i.e task that using cpu  
*/
TCB_Typedef* getCurrentTask(void);

uint32_t getSystemTime(void);

#endif