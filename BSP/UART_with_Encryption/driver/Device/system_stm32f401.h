#ifndef __SYSTEM_STM32F401_H
#define __SYSTEM_STM32F401_H

extern uint32_t SystemCoreClock;
extern uint32_t APB1peripheralClock;
extern uint32_t APB2peripheralClock;

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#endif /*__SYSTEM_STM32F401_H */