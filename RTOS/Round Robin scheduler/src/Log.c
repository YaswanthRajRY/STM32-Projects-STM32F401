#include "Log.h"

/************************************** Display integer *****************************************/
void Log_i(uint32_t data_Tx)
{
    char string[10];
    sprintf(string, "%lX", data_Tx);
    Log_s(string);
}

/************************************** Display pointer *****************************************/
void Log_p(uint32_t* data_Tx)
{
    char string[10];
    sprintf(string, "%lX", (uint32_t)data_Tx);
    Log_s(string);
}

/*************************************** Display string *****************************************/
void Log_s(char* data_Tx)
{
    while (*data_Tx)
    {
        Log_c(*data_Tx);
        data_Tx++;
    }
    Log_c('\n');
    Log_c('\r');
}

/*************************************** Display char ******************************************/
void Log_c(uint8_t Tx_data)
{
    while (!(USART1->SR & USART_SR_TXE));

    USART1->DR = Tx_data;
}


/*************************************** Initialize *******************************************/
void Log_init()
{
    RCC->AHB1ENR |= 1;                      // enable gpio port A clock

    GPIOA->MODER |= (0xA << 18);            // mode as alternate function 
    GPIOA->OSPEEDR |= (0xA << 18);          // high speed mode
    GPIOA->AFR[1] |= (0x77 << 4);           // AF7 for UART 1

    // Enable UART clock
    RCC->APB2ENR |= 0x00000010;             // USART 1 clock enable

    // enable UE
    USART1->CR1 |= 0x00002000;

    // Configure number stop bit
    USART1->CR2 &= ~USART_CR2_STOP;         // 1 stop bit

    // Configure stop bit
    USART1->CR2 |= 0x00003000;              // 1 stop bit

    // Baud rate
    USART1->BRR = (PERIPHERAL_CLK/9600) + 1;

    // Configure UART mode
    USART1->CR1 |= 0x00000008;              // enable Tx
}