#include "stm32f401xc.h"

void timer11_init();
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void uart_send_msg_app(char*);

int main()
{
    timer11_init();

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;
    GPIOA->AFR[1] |= (7 << 4) | (7 << 8);

    USART1->BRR = 16000000 / 9600;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;

    uart_send_msg_app("\n\rRunning blink...\n\r");

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~(3 << 26);
    GPIOC->MODER |= (1 << 26);

    while (1)
    {
        GPIOC->ODR ^= (1 << 13);

        Delay_ms(1000);
    }
}

void timer11_init()
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    
    TIM11->PSC = 15;
    TIM11->ARR = 0xFFFF;
    TIM11->CR1 |= TIM_CR1_CEN;
}

void Delay_us(uint16_t us)
{
    TIM11->CNT = 0;
    while (TIM11->CNT < us);
}

void Delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        Delay_us(1000);
    }
}

void uart_send_msg_app(char* s)
{
    while(*s)
    {
        while (!(USART1->SR & USART_SR_TXE));

        USART1->DR = (uint8_t)*s;
        s++;
    }
}
