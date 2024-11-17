#include "UART.h"

/*************************************** Setup UART *******************************************/
void UART_init(USART_TypeDef* UART, UART_Typedef* uartConfig)
{
    // Enable UART clock
    if ((void*)UART == (void*)UART1)
    {
        RCC->APB2ENR |= 0x00000010;     // USART 1 clock enable
        #if UART1_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(USART1_IRQn);
        __enable_irq();
        #endif
    }
    else if ((void*)UART == (void*)UART6)
    {
        RCC->APB2ENR |= 0x00000020;     // USART 6 clock enable
        #if UART6_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(USART6_IRQn);
        __enable_irq();
        #endif
    }
    else if ((void*)UART == (void*)UART2)
    {
        RCC->APB1ENR |= 0x00020000;     // USART 2 clock enable
        #if UART2_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(USART2_IRQn);
        __enable_irq();
        #endif
    }

    // enable UE
    UART->CR1 |= 0x00002000;

    // Configure number stop bit
    if (uartConfig->NoStopBit == 2)
    {
        UART->CR2 = USART_CR2_STOP_1;   // 2 stop bit
    }
    else
    {
        UART->CR2 &= ~USART_CR2_STOP;   // 1 stop bit
    }

    // Configure parity
    if (uartConfig->ParityEnable)
    {
        UART->CR1 |= 0x00000400;                // Enable parity control
        UART->CR1 |= uartConfig->Parity << 9;   // set odd or even parity
    }

    // Configure stop bit
    UART->CR2 |= 0x00003000;    // 1 stop bit

    // Baud rate
    UART->BRR = (uartConfig->peripheralClock/uartConfig->baudRate)+1;

    // Configure UART mode
    switch (uartConfig->mode)
    {
        case UART_TX:   UART->CR1 |= 0x00000008;        // enable Tx
                        break;
        
        case UART_RX:   UART->CR1 |= 0x00000004;        // enable Rx
                        break;

        case UART_TX_RX:    UART->CR1 |= 0x0000000C;    // enable both Tx & Rx
                            break;
    }
}

/************************************** Transmit data ******************************************/
void UART_Write(USART_TypeDef* UART, uint8_t Tx_data)
{
    while (!(UART->SR & USART_SR_TXE));

    UART->DR = Tx_data;
}

/************************************** Receive data *******************************************/
uint8_t UART_Read(USART_TypeDef* UART)
{
    while (!(UART->SR & USART_SR_RXNE));

    return UART->DR;
}

/************************************* Enable interrupts ****************************************/
void UART_EnableInterrupts(USART_TypeDef* UART)
{
    UART->CR1 |= USART_CR1_TXEIE | USART_CR1_RXNEIE;
}

// Tx interrupt enable
void UART_EnableInterrupts_Tx(USART_TypeDef* UART)
{
    UART->CR1 |= USART_CR1_TXEIE;
}

// Rx interrupt enable
void UART_EnableInterrupts_Rx(USART_TypeDef* UART)
{
    UART->CR1 |= USART_CR1_RXNEIE;
}

/************************************* ISR for UART Tx & Rx ***************************************/
#if UART1_INTERRUPT_ENABLE
void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&UART1_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&UART1_Buff);
            USART1->DR = Tx_Data;
        }
        else
        {
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }

    if (USART1->SR & USART_SR_RXNE)
    {
        uint8_t Rx_data = USART1->DR;
        if (!(ringBuffer_isFull(&UART1_Buff)))
        {
            ringBuffer_Write(&UART1_Buff, Rx_data);
        }
    }
}
#endif

#if UART2_INTERRUPT_ENABLE
void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&UART2_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&UART2_Buff);
            USART2->DR = Tx_Data;
        }
        else
        {
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
    }

    if (USART2->SR & USART_SR_RXNE)
    {
        uint8_t Rx_data = USART2->DR;
        if (!(ringBuffer_isFull(&UART2_Buff)))
        {
            ringBuffer_Write(&UART2_Buff, Rx_data);
        }
    }
}
#endif

#if UART6_INTERRUPT_ENABLE
void USART6_IRQHandler(void)
{
    if (USART6->SR & USART_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&UART6_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&UART6_Buff);
            USART6->DR = Tx_Data;
        }
        else
        {
            USART6->CR1 &= ~USART_CR1_TXEIE;
        }
    }

    if (USART6->SR & USART_SR_RXNE)
    {
        uint8_t Rx_data = USART6->DR;
        if (!(ringBuffer_isFull(&UART6_Buff)))
        {
            ringBuffer_Write(&UART6_Buff, Rx_data);
        }
    }
}
#endif