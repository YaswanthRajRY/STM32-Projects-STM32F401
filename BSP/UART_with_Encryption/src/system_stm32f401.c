#include "stm32f401xc.h"

#define PLLM (25U)
#define PLLN (252U << 6)
#define PLLP (1U << 17)

#define HSI_VALUE    ((uint32_t)16000000)
#define HSE_VALUE    ((uint32_t)25000000)

uint32_t SystemCoreClock = 16000000;
uint32_t APB1peripheralClock;
uint32_t APB2peripheralClock;

static const uint8_t  AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static const uint8_t  APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

/**
 * @brief This function uses HSE oscillator and PLL uaes as the system clock.
 * 
 * System core clock: 48Mhz
 * APB1 pheripheral clock: 48Mhz
 * APB2 pheripheral clock: 48Mhz
 */
void SetSystemClock(void)
{
    // Enable HSE clock
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    // Set HSE as a PLL clock source
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    // Enable PWR pheripheral clock and set VOS scale as mode 2
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS_1;

    // Set flash latency as 2WS and enable data cache, prefech and instruction cache
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN;

    // Set clock multiplication and division factor
    RCC->PLLCFGR |= PLLM | PLLN | PLLP;

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;

    // Enable PLL and set PLL as system core clock
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);
}

/**
 * @brief Updates SystemCoreClock, APB1peripheralClock and APB2peripheralClock variables
 *        according to Clock Register Values.
 * 
 * @param SystemCoreClock
 * @param APB1peripheralClock
 * @param APB2peripheralClock
*/
void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

    // Get clock source
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp)
    {
        // HSI used as system clock source
        case 0x00:
                    SystemCoreClock = HSI_VALUE;
                    break;

        // HSE used as system clock source
        case 0x04:
                    SystemCoreClock = HSE_VALUE;
                    break;

        // PLL used as system clock source
        case 0x08:
                    pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
                    pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
                    if (pllsource != 0)
                    {
                        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6); // HSE used as PLL clock source
                    }
                    else
                    {
                        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6); // HSI used as PLL clock source
                    }

                    pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
                    SystemCoreClock = pllvco/pllp;
                    break;

        default:
                    SystemCoreClock = HSI_VALUE;
                    break;
    }

    // Compute HCLK frequency
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)]; // Get HCLK prescaler
    // HCLK frequency
    SystemCoreClock >>= tmp;

    // Compute ABP1 pheripheral clock
    tmp = APBPrescTable[((RCC->CFGR & RCC_CFGR_PPRE1) >> 10)]; // Get APB1 prescaler
    // APB1 peripheral clock
    APB1peripheralClock = SystemCoreClock >> tmp;

    // Compute APB2 pheripheral clock
    tmp = APBPrescTable[((RCC->CFGR & RCC_CFGR_PPRE2) >> 13)]; // Get APB1 prescaler
    // APB1 peripheral clock
    APB2peripheralClock = SystemCoreClock >> tmp;
}

void SystemInit(void)
{
    SetSystemClock();
    SystemCoreClockUpdate();
}