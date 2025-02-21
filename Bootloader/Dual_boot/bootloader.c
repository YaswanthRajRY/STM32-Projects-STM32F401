#include "stm32f401xc.h"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata; 
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

void uart_send_msg(char*);

void bootloader_Reset_Handler()
{
    __disable_irq();

    uint32_t* src = &_sidata;
    uint32_t* dest = &_sdata;

    // move .data section from flash to sram
    while (dest < &_edata)
    {
        *dest++ = *src++;
    }

    dest = &_sbss;
    // initialze zero in .bss section
    while (dest < &_ebss)
    {
        *dest++ = 0;
    }

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;
    GPIOA->AFR[1] |= (7 << 4) | (7 << 8);

    USART1->BRR = 16000000 / 9600;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;

    uart_send_msg("\n\rRunning Bootloader:\n\n\rPress button to run application 1");

    RCC->AHB1ENR |= 0x1; // Enable GPIO A clock
    GPIOA->MODER &= ~(0x3); // Set as input mode
    GPIOA->PUPDR |= 0x1; // pull-down

    uint32_t timeout = 0;
    uint8_t flag = 1;

    while(GPIOA->IDR & 1)
    {
        timeout += 2;
        
        if (timeout >= 4000000) // 3-4 sec delay
        {
            flag = 0;
            break;
        }
    }

    uint32_t APP_ADDR;

    if (!flag)
    {
        APP_ADDR = 0x08004000;  // (sector 2) application program 1
    }
    else
    {
        APP_ADDR = 0x08008000; // (sector 3) application program 2
    }
  
    SCB->VTOR = APP_ADDR; // relocate vector table offset register

    __set_MSP(*(volatile uint32_t*)(APP_ADDR)); // set main stack pointer

    // copy application reset handler address from vector table
    void (*app_reset_handler)(void) = (void (*)(void)) (*(volatile uint32_t*) (APP_ADDR + 4)); // casting into an function pointer

    app_reset_handler(); // Jump to application firmware
}

void uart_send_msg(char* s)
{
    while(*s)
    {
        while (!(USART1->SR & USART_SR_TXE));

        USART1->DR = (uint8_t)*s;
        s++;
    }
}

__attribute__((section(".isr_vector"))) uint32_t bootloader_vector_table[] = {
    (uint32_t)&_estack,   // Initial MSP value
    (uint32_t)&bootloader_Reset_Handler, // Reset Handler
};