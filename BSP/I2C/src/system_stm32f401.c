#include "main.h"

#define PLLM            (25U)
#define PLLN            (252U << 6)
#define PLLP            (1U << 17)
#define PSC_VALUE       41
#define ARR_VALUE       0xFFFF

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
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;            // enable gpio port A clock

    // Configure PB6 and PB7 as Alternate Function (AF4 for I2C1)
    GPIOB->MODER &= ~((3U << GPIO_MODER_MODER6_Pos)
                    | (3U << GPIO_MODER_MODER7_Pos));           // Clear mode bits

    GPIOB->MODER |= (2U << GPIO_MODER_MODER6_Pos)
                    | (2U << GPIO_MODER_MODER7_Pos);            // Set to Alternate Function

    GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;         // Open-drain

    GPIOB->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED6_Pos)
                    | (3U << GPIO_OSPEEDR_OSPEED7_Pos);         // High speed

    GPIOB->PUPDR &= ~((3U << GPIO_PUPDR_PUPD6_Pos)
                    | (3U << GPIO_PUPDR_PUPD7_Pos));            // No pull-up, no pull-down

    GPIOB->AFR[0] |= (4U << GPIO_AFRL_AFSEL6_Pos)
                    | (4U << GPIO_AFRL_AFSEL7_Pos);             // Alternate function 4 (AF4) for I2C1

    // Configure PB8 and PB9 as Alternate Function (AF4 for I2C2)
    GPIOB->MODER &= ~((3U << GPIO_MODER_MODER8_Pos)
                    | (3U << GPIO_MODER_MODER9_Pos));           // Clear mode bits

    GPIOB->MODER |= (2U << GPIO_MODER_MODER8_Pos) 
                    | (2U << GPIO_MODER_MODER9_Pos);            // Set to Alternate Function

    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;         // Open-drain

    GPIOB->OSPEEDR |= (3U << GPIO_OSPEEDR_OSPEED8_Pos)
                    | (3U << GPIO_OSPEEDR_OSPEED9_Pos);         // High speed

    GPIOB->PUPDR &= ~((3U << GPIO_PUPDR_PUPD8_Pos)
                    | (3U << GPIO_PUPDR_PUPD9_Pos));            // No pull-up, no pull-down

    GPIOB->AFR[1] |= (4U << GPIO_AFRH_AFSEL8_Pos)
                    | (4U << GPIO_AFRH_AFSEL9_Pos);             // Alternate function 4 (AF4) for I2C2
}

/***************************************** Function to configure Timer ****************************************/
static void timerConfig()
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;            // Enable Timer 11

    TIM11->PSC = PSC_VALUE;                         // Set Prescaler value
    
    TIM11->ARR = ARR_VALUE;                         // Set Auto-reload value

    TIM11->CR1 |= 0x1;                              // Counter enable

    while (!(TIM11->SR & 1));                       // Update interrupt flag
}

/***************************************** Function to create 1ms delay ***************************************/
void Delay_us(uint16_t us)
{
    TIM11->CNT = 0;                                 // reset counter value
    while (TIM11->CNT < us);
}

/***************************************** Function to create 1us delay ***************************************/
void Delay_ms(uint16_t ms)
{
    for (uint16_t i=0; i<ms; i++)
    {
        Delay_us(1000);                             // for 1ms
    }
}

/************************************** Function to initalize all config ***************************************/
void SystemInit(void)
{
    SysClockConfig_42Mhz();                         // configure clock
    gpioConfig();                                   // configure port b
    timerConfig();                                  // timer configuration for delay
}

void SystemCoreClockUpdate(void)
{

}