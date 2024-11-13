#ifndef BSP_H_
#define BSP_H_

#include "stm32f401.h"

#define UART1 USART1_BASE_ADDR
#define UART2 USART2_BASE_ADDR
#define UART6 USART6_BASE_ADDR

enum UART_mode
{
    UART_TX,
    UART_RX,
    UART_TX_RX
};

typedef struct 
{
    uint32_t baudRate;
    uint32_t peripheralClock;
    uint8_t mode;
    uint8_t ParityEnable;
    uint8_t Parity;
}UART_Typedef;

void UART_init(USART_TypeDef* UART, UART_Typedef* uartConfig);
void UART_send(USART_TypeDef* UART, uint8_t Tx_data);
uint8_t UART_receive(USART_TypeDef* UART);

#endif