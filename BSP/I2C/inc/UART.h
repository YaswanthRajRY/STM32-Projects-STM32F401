#ifndef BSP_H_
#define BSP_H_

#include "stm32f401xc.h"
#include "ringBuffer.h"

/*
    macros contains UART address
*/
#define UART1 (void*)USART1_BASE
#define UART2 (void*)USART2_BASE
#define UART6 (void*)USART6_BASE

/*
    set value as 1 to enable interrupt
*/
#define UART1_INTERRUPT_ENABLE 0
#define UART2_INTERRUPT_ENABLE 0
#define UART6_INTERRUPT_ENABLE 0

/*
    use UARTx_Buff to create ring buffer
    ex: (in main code)
    ringBuffer_Typedef UART1_Buff;
*/
#if UART1_INTERRUPT_ENABLE
extern ringBuffer_Typedef UART1_Buff;
#endif
#if UART2_INTERRUPT_ENABLE
extern ringBuffer_Typedef UART2_Buff;
#endif
#if UART6_INTERRUPT_ENABLE
extern ringBuffer_Typedef UART6_Buff;
#endif

/*
    use declared enum to select UART mode
*/
enum UART_mode
{
    UART_TX,
    UART_RX,
    UART_TX_RX
};

/*
    use declared struct to configure UART function
*/
typedef struct 
{
    uint32_t baudRate;
    uint32_t peripheralClock;
    uint8_t mode;
    uint8_t ParityEnable;
    uint8_t Parity;
    uint8_t NoStopBit;
}UART_Typedef;

/*
    function declaration
*/
void UART_init(USART_TypeDef* UART, UART_Typedef* uartConfig);
void UART_Write(USART_TypeDef* UART, uint8_t Tx_data);
void UART_EnableInterrupts(USART_TypeDef* UART);
void UART_EnableInterrupts_Tx(USART_TypeDef* UART);
void UART_EnableInterrupts_Rx(USART_TypeDef* UART);

uint8_t UART_Read(USART_TypeDef* UART);

#endif
