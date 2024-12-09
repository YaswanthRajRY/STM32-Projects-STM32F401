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
    RCC->AHB1ENR |= 1;                      // enable gpio port A clock
    RCC->AHB1ENR |= (1 << 1);               // enable gpio port B clock
   
    // For SPI-1 port A
    // mode as alternate function
    GPIOA->MODER |= GPIO_MODER_MODE4_1 |                // NSS
                    GPIO_MODER_MODE5_1 |                // Clk
                    GPIO_MODER_MODE6_1 |                // MISO
                    GPIO_MODER_MODE7_1;                 // MOSI
    
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD6_1;                 // MISO Set Pull-Up
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD4_0;                 // NSS Set Pull-Up

    // high speed mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4_1 |        // NSS
                      GPIO_OSPEEDER_OSPEEDR5_1 |        // Clk
                      GPIO_OSPEEDER_OSPEEDR6_1 |        // MISO
                      GPIO_OSPEEDER_OSPEEDR7_1;         // MOSI

    // AF5 for SPI 1
    GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL4_Pos) |      // NSS
                     (5 << GPIO_AFRL_AFSEL5_Pos) |      // Clk
                     (5 << GPIO_AFRL_AFSEL6_Pos) |      // MISO
                     (5 << GPIO_AFRL_AFSEL7_Pos);       // MOSI

    // For SPI-2 port B
    // mode as alternate function
    GPIOB->MODER |= GPIO_MODER_MODE12_1 |               // NSS
                    GPIO_MODER_MODE13_1 |               // Clk
                    GPIO_MODER_MODE14_1 |               // MISO
                    GPIO_MODER_MODE15_1;                // MOSI
    
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD14_1;                // MISO Set Pull-Up
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD12_0;                // NSS Set Pull-Up

    // high speed mode
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1 |       // NSS
                      GPIO_OSPEEDER_OSPEEDR13_1 |       // Clk
                      GPIO_OSPEEDER_OSPEEDR14_1 |       // MISO
                      GPIO_OSPEEDER_OSPEEDR15_1;        // MOSI

    // AF5 for SPI 2
    GPIOB->AFR[1] |= (5 << GPIO_AFRH_AFSEL12_Pos) |     // NSS
                     (5 << GPIO_AFRH_AFSEL13_Pos) |     // Clk
                     (5 << GPIO_AFRH_AFSEL14_Pos) |     // MISO
                     (5 << GPIO_AFRH_AFSEL15_Pos);      // MOSI

    GPIOA->MODER |= (0xA << 18);            // mode as alternate function 
    GPIOA->OSPEEDR |= (0xA << 18);          // high speed mode
    GPIOA->AFR[1] |= (0x77 << 4);           // AF7 for UART 1
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