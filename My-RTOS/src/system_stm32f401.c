#include "main.h"

#define PLLM            (25U)
#define PLLN            (252U << 6)
#define PLLP            (1U << 17)

/************************************* Function to configure system clock **************************************/
static void SysClockConfig_42Mhz()
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

/****************************************** Function to configure GPIO *****************************************/
static void gpioConfig()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;            // Enable Port B clock

    GPIOB->MODER |= GPIO_MODER_MODE5_0
                  | GPIO_MODER_MODE6_0
                  | GPIO_MODER_MODE7_0;             // Configure as General purpose output mode
}

/************************************** Function to initalize all config ***************************************/
void SystemInit(void)
{
    SysClockConfig_42Mhz();             // configure clock
    gpioConfig();                       // configure port b
    Log_init();                         // initialize UART 1 to use as Log for debugging
}

void SystemCoreClockUpdate(void)
{

}
