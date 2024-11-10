#include "stm32f401ccu6.h"

#define PLLM            (25U)
#define PLLN            (252U << 6)
#define PLLP            (1U << 17)
#define MODER_PIN_13    (1U << 26)
#define TIM11_EN        (1U << 18)
#define PSC_VALUE       41
#define ARR_VALUE       0xFFFF
#define SET_PIN_13      (1U << 13)
#define RESET_PIN_13    (1U << 29)
#define GPIOC_EN        (0x1U << 2)
#define SysCoreClk      42000000UL

void SysClockConfig_42Mhz(void);
void timerConfig(void);
void GPIOconfig(void);
void Delay_us(uint16_t);
void Delay_ms(uint16_t);
void UART1_init(uint32_t);
void UART1_Tx(uint8_t);

int main(void)
{
    SysClockConfig_42Mhz();
    GPIOconfig();
    timerConfig();
    UART1_init(9600);

    while (1)
    {
        char* s = "Hello World\n\r";
        while (*s)
        {
            UART1_Tx(*s);
            s++;
        }
        Delay_ms(1000);                                                    // 1sec delay
    }
}

void SysClockConfig_42Mhz()
{
    RCC->CR |= HSE_ON;
    while (!(RCC->CR & HSE_RDY));
    
    RCC->PLLCFGR |= PLLSRC_HSE;

    RCC->APB1ENR |= PWR_EN;
    PWR->CR |= VOS_SCALE_2_MODE;

    FLASH->ACR |= LATENCY_2WS | DCEN | PRFTEN | ICEN;

    RCC->PLLCFGR |= PLLM | PLLN | PLLP;
    RCC->CFGR |= AHB_PRE_DIV1 | APB1_PRE_DIV2 | APB2_PRE_DIV1;

    RCC->CR |= PLLON;
    while (!(RCC->CR & PLLRDY));

    RCC->CFGR |= SW_PLL;
    while ((RCC->CFGR & SWS) != SW_PLL_MASK);
}

void GPIOconfig()
{
    RCC->AHB1ENR |= GPIOC_EN;                                    // Enable GPIO PORT C

    GPIOC->MODER |= MODER_PIN_13;                                           // Set PIN 13 of PORT C as general purpose output mode

    GPIOC->OTYPER = 0;                                                      // Set output type as no pull-push
    GPIOC->OSPEEDR = 0;                                                     // Set low speed
}

void timerConfig()
{
    RCC->APB2ENR |= TIM11_EN;                                               // Enable Timer 11

    TIM11->PSC = PSC_VALUE;                                                 // Set Prescaler value
    
    TIM11->ARR = ARR_VALUE;                                                 // Set Auto-reload value

    TIM11->CR1 |= 0x1;                                                      // Counter enable

    while (!(TIM11->SR & 1));                                               // Update interrupt flag
}

void Delay_us(uint16_t us)
{
    TIM11->CNT = 0;                                                         // reset counter value
    while (TIM11->CNT < us);
}

void Delay_ms(uint16_t ms)
{
    for (uint16_t i=0; i<ms; i++)
    {
        Delay_us(1000);                                                    // for 1ms
    }
}

void UART1_init(uint32_t BaudRate)
{
    RCC->AHB1ENR |= 1;
    RCC->APB2ENR |= (1 << 4);
    GPIOA->MODER |= (0xA << 18);
    GPIOA->OSPEEDR |= (0xA << 18);
    GPIOA->AFR[1] = (0x77 << 4);

    UART1->CR1 |= (1 << 13);
    UART1->CR2 &= ~(3 << 12);
    UART1->BRR = (SysCoreClk/BaudRate) + 1;
    UART1->CR1 |= (1 << 3);
}

void UART1_Tx(uint8_t data)
{
    //while (!(USART1_SR & (1 << 7)));

    UART1->DR = data;
    while (!(UART1->SR & (1 << 6)));
}