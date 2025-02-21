#include "stm32f401xc.h"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata; 
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

void uart_send_msg(char*);
void update_firmware();
void erase_sector();
void uart_init();
uint16_t uart_receive(uint8_t*, uint16_t);

void bootloader_Reset_Handler()
{
    uint32_t* src = &_sidata;
    uint32_t* dest = &_sdata;

    while (dest < &_edata)
    {
        *dest++ = *src++; // move .data section from flash to sram
    }

    dest = &_sbss;
    
    while (dest < &_ebss)
    {
        *dest++ = 0; // initialze zero in .bss section
    }

    uart_init(); // initialize uart

    uart_send_msg("\n\rRunning Bootloader: v2.0\n\r");

    RCC->AHB1ENR |= 0x1; // Enable GPIO A clock
    GPIOA->MODER &= ~(0x3); // Set as input mode
    GPIOA->PUPDR |= 0x1; // pull-down

    uint32_t timeout = 0;
    uint8_t flag = 1;

    while(GPIOA->IDR & 1)
    {
        timeout += 2;
        
        if (timeout >= 1600000) // 3-4 sec delay
        {
            flag = 0;
            break;
        }
    }

    if (flag)
    {
        uart_send_msg("\n\rEntered firmware update mode\n\r");
        update_firmware();
    }
    
    uint32_t APP_ADDR = 0x08004000; // Flash sector 1 starting address
  
    SCB->VTOR = APP_ADDR; // relocate vector table offset register

    __set_MSP(*(volatile uint32_t*)(APP_ADDR)); // set main stack pointer

    // copy application reset handler address from vector table
    void (*app_reset_handler)(void) = (void (*)(void)) (*(volatile uint32_t*) (APP_ADDR + 4)); // casting into an function pointer

    uart_send_msg("\n\rExecuting application program\n\r");

    app_reset_handler(); // Jump to application firmware
}

__attribute__((section(".isr_vector"))) uint32_t bootloader_vector_table[] = {
    (uint32_t)&_estack,   // Initial MSP value
    (uint32_t)&bootloader_Reset_Handler, // Reset Handler
};

void uart_init()
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable USART peripheral clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock 

    GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1; // Set Alternate function mode
    GPIOA->AFR[1] |= (7 << 4) | (7 << 8); // Set pin 9 & 10 for USART

    USART1->BRR = 16000000 / 9600; // Set baudrate
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable Tx, Rx and start usart
}

void uart_send_msg(char* s)
{
    while(*s)
    {
        while (!(USART1->SR & USART_SR_TXE)); // wait till data register is empty

        USART1->DR = (uint8_t)*s; // Copy data to data register
        s++;
    }
}

void update_firmware()
{
    // Unlock flash
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    erase_sector(); // Function to erase sector 1
    
    uint32_t addr = 0x08004000;
    uint8_t buffer[256];
    
    FLASH->CR |= FLASH_CR_PSIZE_1; // write 4 bytes
    FLASH->CR |= FLASH_CR_PG; // Enable flash programming mode

    while (1)
    {
        uint16_t received_byte = uart_receive(buffer, 256);
        
        if (received_byte > 0)
        {
            for (uint16_t i=0; i<received_byte; i += 4)
            {
                *(volatile uint32_t*)addr = *(uint32_t*)&buffer[i]; // Copy data to flash
                while (FLASH->SR & FLASH_SR_BSY); // wait till write is complete
                addr += 4;
            }
        }
        else
        {
            uart_send_msg("Flashing completed\n\r");
            break;
        }
    }

    FLASH->CR &= ~FLASH_CR_PG; // Disable flash programming mode
    FLASH->CR |= FLASH_CR_LOCK; // Lock flash
}

void erase_sector()
{
    uart_send_msg("\n\rErasing Sector 1\t");

    while (FLASH->SR & FLASH_SR_BSY); // wait if flash is busy
    
    FLASH->CR |= FLASH_CR_SER; // acivate flash erase mode
    FLASH->CR |= FLASH_CR_SNB_0; // select sector 1
    FLASH->CR |= FLASH_CR_STRT; // start erasing
    
    while (FLASH->SR & FLASH_SR_BSY); // wait till ease complete
    
    FLASH->CR &= ~FLASH_CR_SER; // deactivate erase mode
    
    uart_send_msg("Erase completed\n\r");
}

uint16_t uart_receive(uint8_t* buffer, uint16_t size)
{
    uint16_t return_size = 0;
    while (return_size < size)
    {
        uint32_t timeout = 0;
        while (!(USART1->SR & USART_SR_RXNE))
        {
            timeout++;
            if (timeout >= 1600000) // 3 sec delay
                return return_size; // Return no data received
        }
        
        buffer[return_size] = USART1->DR; // Copy received data to buffer
        return_size++;
    }

    return return_size; // Return num of bytes received
}