#include "stm32f401xc.h"
#include "UART.h"
#include "ringBuffer.h"

#define PLLM            (25U)
#define PLLN            (252U << 6)
#define PLLP            (1U << 17)
#define PSC_VALUE       41
#define ARR_VALUE       0xFFFF
#define SysCoreClk      42000000UL

void SysClockConfig_42Mhz(void);
void timerConfig(void);
void GPIOconfig(void);
void Delay_us(uint16_t);
void Delay_ms(uint16_t);
void UART1_Tx(uint8_t);