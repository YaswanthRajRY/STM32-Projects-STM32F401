#include "uart.h"

/**
 * @brief uart1_init function initializes uart 1 (pin: A9 A10) in Tx & Rx mode.
 *        also initializes GPIOA for uart function.
 * 
 * @param baud_rate argument passed to the function used to set user defined baudrate
 * 
 * @param APB2peripheralClock has value of APB2 peripheral clock
 */
void uart1_init(uint32_t baud_rate)
{
    // Enable GPIO and USART1 peripheral clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Configure GPIOA pin 9 & 10 as alternate function mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE9);
    GPIOA->MODER |= GPIO_MODER_MODE10_1 | GPIO_MODER_MODE9_1;
    GPIOA->AFR[1] &= ~((0xF << (1 * 4)) | (0xF << (2 * 4)));
    GPIOA->AFR[1] |= (7 << (1 * 4)) | (7 << (2 * 4));

    // Configure USART1
    USART1->BRR = (APB2peripheralClock + baud_rate / 2) / baud_rate;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

/**
 * @brief uart2_init function initializes uart 2 (pin: A2 A3) in Tx & Rx mode.
 *        also initializes GPIOA for uart function.
 * 
 * @param baud_rate argument passed to the function used to set user defined baudrate
 * 
 * @param APB1peripheralClock has value of APB1 peripheral clock
 */
void uart2_init(uint32_t baud_rate)
{
    // Enable GPIO and USART2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configure GPIOA pin 2 & 3 as alternate function mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
    GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFR[0] |= (7 << (2 * 4)) | (7 << (3 * 4));

    // Configure USART1
    USART2->BRR = (APB1peripheralClock + baud_rate / 2) / baud_rate;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

/**
 * @brief uart6_init function initializes uart 6 (pin: A11 A12) in Tx & Rx mode.
 *        also initializes GPIOA for uart function.
 * 
 * @param baud_rate argument passed to the function used to set user defined baudrate
 * 
 * @param APB2peripheralClock has value of APB2 peripheral clock
 */
void uart6_init(uint32_t baud_rate)
{
    // Enable GPIO and USART6 peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    // Configure GPIOA pin 11 & 12 as alternate function mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
    GPIOA->MODER |= GPIO_MODER_MODE11_1 | GPIO_MODER_MODE12_1;
    GPIOA->AFR[1] &= ~((0xF << (3 * 4)) | (0xF << (4 * 4)));
    GPIOA->AFR[1] |= (8 << (3 * 4)) | (8 << (4 * 4));
    
    // Configure USART6
    USART6->BRR = (APB2peripheralClock + baud_rate / 2) / baud_rate;
    USART6->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    
}