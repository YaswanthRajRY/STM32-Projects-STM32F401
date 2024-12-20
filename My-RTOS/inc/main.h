#include <stdio.h>
#include <string.h>

#include "task.h"
#include "Log.h"
#include "mutex.h"
#include "scheduler.h"
#include "stm32f401xc.h"

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);