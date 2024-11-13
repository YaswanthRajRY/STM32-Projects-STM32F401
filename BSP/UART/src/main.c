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
//void UART1_init(uint32_t);
void UART1_Tx(uint8_t);

int main(void)
{
    SysClockConfig_42Mhz();
    GPIOconfig();
    timerConfig();
    
    UART_Typedef UART1_config = {
        .baudRate = 9600,       // baud rate
        .peripheralClock = SysCoreClk,  // peripheral clock
        .mode = UART_TX,        // UART communication mode
        .ParityEnable = 0,      // enable parity
        .Parity = 0     // even parity
    };

    UART_init(USART1_BASE_ADDR, &UART1_config);


    while (1)
    {
        char* s = "Hello World :)\n\r";
        while (*s)
        {
            UART_send(UART1, *s);
            s++;
        }
        Delay_ms(1000);      // 1sec delay
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
    RCC->AHB1ENR |= 1;      // enable gpio port A clock
    GPIOA->MODER |= (0xA << 18);    
    GPIOA->OSPEEDR |= (0xA << 18);
    GPIOA->AFR[1] = (0x77 << 4);
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