#include "main.h"

UART_Typedef UART1_config = {
        .baudRate = 9600,               // baud rate
        .peripheralClock = SysCoreClk,  // peripheral clock
        .mode = UART_TX,                // UART communication mode
        .ParityEnable = 0,              // enable parity
        .Parity = 0,                    // even parity
        .NoStopBit = 1                  // stop bit
};

UART_Typedef UART6_config = {
        .baudRate = 9600,               // baud rate
        .peripheralClock = SysCoreClk,  // peripheral clock
        .mode = UART_TX_RX,             // UART communication mode
        .ParityEnable = 0,              // enable parity
        .Parity = 0,                    // even parity
        .NoStopBit = 1                  // stop bit
};

ringBuffer_Typedef UART1_Buff;
ringBuffer_Typedef UART6_Buff;

int main(void)
{
    SysClockConfig_42Mhz();             // set clock

    GPIOconfig();                       // config gpio ports for UART opertion

    timerConfig();                      // timer configuration for delay

    UART_init(UART1, &UART1_config);    // setup UART 1
    UART_init(UART6, &UART6_config);    // setup UART 1

    ringBuffer_init(&UART1_Buff);       // setup ring buffer for UART 1 
    ringBuffer_init(&UART6_Buff);       // setup ring buffer for UART 2

    while (1)
    {
        char* s = "Vanakam da maplai! UART 1 la irunthu :)\n\r";
        while (*s)
        {
            ringBuffer_Write(&UART1_Buff, *s);          // write data into buffer 
            s++;
        }
        UART_EnableInterrupts(UART1);                   // enable interrupts to start transmission

        UART_EnableInterrupts(UART6);                   // enable interrupts to receive incoming data
        while (!ringBuffer_isEmpty(&UART6_Buff))
        {
            uint8_t data = ringBuffer_Read(&UART6_Buff);
            ringBuffer_Write(&UART6_Buff, data);        // write received data into UART 6 buffer
        }
        
        Delay_ms(1000);                                 // 1sec delay
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