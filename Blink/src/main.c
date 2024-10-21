#include "main.h"
#include <stdint.h>

void sysClkConfig(void);
void timerConfig(void);
void GPIOconfig(void);
void Delay_us(uint16_t);
void Delay_ms(uint16_t);

int main(void)
{
    sysClkConfig();
    GPIOconfig();
    timerConfig();

    while(1)
    {
        GPIOC_BSRR |= SET_PIN_13;                                          // Set pin 13 high  
        Delay_ms(1000);                                                    // 1sec delay

        GPIOC_BSRR |= RESET_PIN_13;                                        // Set pin 13 low
        Delay_ms(1000);                                                    // 1sec delay
    }
}

void sysClkConfig()
{
    RCC_CR |= RCC_CR_HSION;                                                // Enable High Speed Internal clock
    while (!(RCC_CR & RCC_CR_HSIRDY));                                     // Wait for it to became ready to use
    
    RCC_APB1ENR |= RCC_APB1ENR_PWREN;                                      // Reset all interface
    PWR_CR |= PWR_CR_VOS;                                                  // Reset Voltage regulator

    FLASH_ACR |= FLASH_ACR_LATENCY_1WS | FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;   // Set Flash latency and enable Data cache, Instruction ache and Prefech catch enable

    RCC_CFGR |= RCC_CFGR_HPRE_DIV2;                                        // Set prescaler
    RCC_CFGR |= RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;                 // Set prescaler
    RCC_PLLCFGR = (1 << 4) | (336 << 6) | (1 << 16);                       // Configure PLL prescaler

    RCC_CR |= RCC_CR_PLLON;                                                // Enable PLL
    while (!(RCC_CR & RCC_CR_PLLRDY));                                     // Wait for PLL to be ready

    RCC_CFGR |= RCC_CFGR_SW_PLL;                                           // Set PLL as Sys Clock
    while ((RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);                 // Wait until PLL is used as the system clock
}

void GPIOconfig()
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;                                    // Enable GPIO PORT C

    GPIOC_MODER |= MODER_PIN_13;                                           // Set PIN 13 of PORT C as general purpose output mode

    GPIOC_OTYPER = 0;                                                      // Set output type as no pull-push
    GPIOC_OSPEEDR = 0;                                                     // Set low speed
}

void timerConfig()
{
    RCC_APB2ENR |= TIM11_EN;                                               // Enable Timer 11

    TIM11_PSC = PSC_VALUE;                                                 // Set Prescaler value
    
    TIM11_ARR = ARR_VALUE;                                                 // Set Auto-reload value

    TIM11_CR1 |= 0x1;                                                      // Counter enable

    while (!(TIM11_SR & 1));                                               // Update interrupt flag
}

void Delay_us(uint16_t us)
{
    TIM11_CNT = 0;                                                         // reset counter value
    while (TIM11_CNT < us);
}

void Delay_ms(uint16_t ms)
{
    for (uint16_t i=0; i<ms; i++)
    {
        Delay_us(1000);                                                    // for 1ms
    }
}