#include "Log.h"

// Function to convert an integer to a string
static void intToString(uint32_t num, char *str) {
    int i = 0, isNegative = 0;

    // Handle the case where num is 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';  // Null-terminate the string
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;  // Convert to positive for further processing
    }

    // Extract digits and store them in reverse order
    while (num > 0) {
        str[i++] = (num % 10) + '0';  // Get the last digit and convert to char
        num /= 10;                    // Remove the last digit
    }

    // Add the negative sign if needed
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string to get the correct order
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/************************************** Display integer *****************************************/
void Log_i(uint32_t data_Tx)
{
    char string[10];
    intToString(data_Tx, string);
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