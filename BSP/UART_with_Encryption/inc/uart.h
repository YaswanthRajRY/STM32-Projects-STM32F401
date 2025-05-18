#ifndef UART_H
#define UART_H

#include "stm32f401xc.h"

void uart1_init(uint32_t baud_rate);
void uart2_init(uint32_t baud_rate);
void uart6_init(uint32_t baud_rate);

#endif