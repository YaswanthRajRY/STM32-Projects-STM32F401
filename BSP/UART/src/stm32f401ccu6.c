#include "stm32f401ccu6.h"

void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);

void UART_init(USART_TypeDef* UART, UART_Typedef* uartConfig)
{
    // Enable UART clock
    if ((void*)UART == (void*)USART1_BASE_ADDR)
    {
        RCC->APB2ENR |= 0x00000010;     // USART 1 clock enable
    }
    else if ((void*)UART == (void*)USART6_BASE_ADDR)
    {
        RCC->APB2ENR |= 0x00000020;     // USART 6 clock enable
    }
    else
    {
        RCC->APB1ENR |= 0x00020000;     // USART 2 clock enable
    }

    // enable UE
    UART->CR1 |= 0x00002000;

    // Configure parity
    if (uartConfig->ParityEnable)
    {
        UART->CR1 |= 0x00000400;    // Enable parity control
        UART->CR1 |= uartConfig->Parity << 9;   // set odd or even parity
    }

    // Configure stop bit
    UART->CR2 |= 0x00003000;    // 1 stop bit

    // Baud rate
    UART->BRR = (uartConfig->peripheralClock/uartConfig->baudRate) + 1;

    // Configure UART mode
    switch (uartConfig->mode)
    {
        case UART_TX:   UART->CR1 |= 0x00000008;    // enable Tx
        
        case UART_RX:   UART->CR1 |= 0x00000004;    // enable Rx

        case UART_TX_RX:    UART->CR1 |= 0x0000000C;    // enable both Tx & Rx
    }
}

void UART_send(USART_TypeDef* UART, uint8_t Tx_data)
{
    while (!(UART->SR & (1 << 7)));

    UART->DR = Tx_data;
}

uint8_t UART_receive(USART_TypeDef* UART)
{
    while (!(UART->SR & (1 << 5)));

    return UART->DR;
}