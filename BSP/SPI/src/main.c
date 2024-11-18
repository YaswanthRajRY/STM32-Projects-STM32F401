#include "main.h"

SPIconfig_Typedef SPI1_config = {
    .peripheralClock = SysCoreClk,
    .baudRatePrescaler = PRE_2,
    .operationMode = MASTER,
    .dataOrder = 0,
    .dataFrameFormat = Bit_8,
    .SPImode = MODE_0,
    .TIenable = 0
};

int main(void)
{
    SysClockConfig_42Mhz();
    timerConfig();
    GPIOconfig();
    SPI_init(SPI_1, &SPI1_config);

    while (1)
    {
        
    }
}

void SysClockConfig_42Mhz()
{
    RCC->CR |= RCC_CR_HSEON;                        // enable HSE
    while (!(RCC->CR & RCC_CR_HSERDY));             // wait for HSE clock to be ready
    
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;         // set HSE as a PLL clock source

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;              // enable PWR pheripheral clock
    PWR->CR |= PWR_CR_VOS_1;                        // set VOS scale mode 2

    FLASH->ACR |= FLASH_ACR_LATENCY_2WS             // set Flash latency as 2 wait cycle
                  | FLASH_ACR_DCEN                  // enable data cache 
                  | FLASH_ACR_PRFTEN                // enable prefech
                  | FLASH_ACR_ICEN;                 // enable instruction cache

    RCC->PLLCFGR |= PLLM | PLLN | PLLP;             // set multiplication and division factor

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1                 // AHB div 1
                 | RCC_CFGR_PPRE1_DIV1              // APB1 divi 1
                 | RCC_CFGR_PPRE2_DIV1;             // APB2 div 1

    RCC->CR |= RCC_CR_PLLON;                        // enable PLL
    while (!(RCC->CR & RCC_CR_PLLRDY));             // waith for PLL to be ready

    RCC->CFGR |= RCC_CFGR_SW_PLL;                   // set PLL as sys clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);   // check status to confirm PLL is set as sys clock
}

void GPIOconfig()
{
    RCC->AHB1ENR |= 1;                      // enable gpio port A clock

    GPIOA->MODER |= (0xA << 18);            // mode as alternate function 
    GPIOA->OSPEEDR |= (0xA << 18);          // high speed mode
    GPIOA->AFR[1] |= (0x77 << 4);           // AF7 for UART 1

    GPIOA->MODER |= (0xA << 22);            // mode as alternate function 
    GPIOA->OSPEEDR |= (0xA << 22);          // high speed mode
    GPIOA->AFR[1] |= 0x00088000;            // AF7 for UART 1
}

void timerConfig()
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;    // Enable Timer 11

    TIM11->PSC = PSC_VALUE;                 // Set Prescaler value
    
    TIM11->ARR = ARR_VALUE;                 // Set Auto-reload value

    TIM11->CR1 |= 0x1;                      // Counter enable

    while (!(TIM11->SR & 1));               // Update interrupt flag
}

void Delay_us(uint16_t us)
{
    TIM11->CNT = 0;             // reset counter value
    while (TIM11->CNT < us);
}

void Delay_ms(uint16_t ms)
{
    for (uint16_t i=0; i<ms; i++)
    {
        Delay_us(1000);     // for 1ms
    }
}