#include "stm32f401xc.h"

#define FLASH_START_ADDR 0x08004000
#define FLASH_END_ADDR 0x08007FFF
#define CRC_ADDR (FLASH_END_ADDR - 4)
#define APP_END_ADDR (FLASH_END_ADDR - 8)

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
uint8_t check_crc();

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
    GPIOA->PUPDR |= 0x1; // pull-up

    if (!(GPIOA->IDR & 1))
    {
        uart_send_msg("Entered firmware update mode:\n\r");
        update_firmware();
    }
    else
    {
        uart_send_msg("Running firmware integrity test: - - - ");
        if (!check_crc())
        {
            uart_send_msg("test failed!\n\r");
            return;
        }
        uart_send_msg("test passed!:\n\r");
    }
    
    uint32_t APP_ADDR = FLASH_START_ADDR;
  
    SCB->VTOR = APP_ADDR; // relocate vector table offset register

    __set_MSP(*(volatile uint32_t*)(APP_ADDR)); // set main stack pointer

    // copy application reset handler address from vector table
    void (*app_reset_handler)(void) = (void (*)(void)) (*(volatile uint32_t*) (APP_ADDR + 4)); // casting into an function pointer

    uart_send_msg("Jumping to application program.\n\r");

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
    
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
    CRC->CR = CRC_CR_RESET;

    FLASH->CR |= FLASH_CR_PSIZE_1; // write 4 bytes
    FLASH->CR |= FLASH_CR_PG; // Enable flash programming mode

    uart_send_msg("Flash new firmware - - - > ");
    while (1)
    {
        uint16_t received_byte = uart_receive(buffer, 256);
        
        if (received_byte > 0 && received_byte % 4 == 0)
        {
            for (uint16_t i=0; i<received_byte; i += 4)
            {   
                uint32_t receivedData = *(uint32_t*)&buffer[i];
                *(uint32_t*)addr = receivedData; // Copy data to flash
                while (FLASH->SR & FLASH_SR_BSY); // wait till write is complete
                CRC->DR = receivedData;
                addr += 4;
            }
        }
        else
        {
            *(uint32_t*)APP_END_ADDR = addr - FLASH_START_ADDR; // write firmware size
            uart_send_msg("completed :)\n\r");
            break;
        }
    }

    *(uint32_t*)CRC_ADDR = CRC->DR; // write new firmware crc
    while (FLASH->SR & FLASH_SR_BSY);

    FLASH->CR &= ~FLASH_CR_PG; // Disable flash programming mode
    FLASH->CR |= FLASH_CR_LOCK; // Lock flash
}

void erase_sector()
{
    uart_send_msg("Erasing Sector 1 - - - > ");

    while (FLASH->SR & FLASH_SR_BSY); // wait if flash is busy
    
    FLASH->CR |= FLASH_CR_SER; // acivate flash erase mode
    FLASH->CR |= FLASH_CR_SNB_0; // select sector 1
    FLASH->CR |= FLASH_CR_STRT; // start erasing
    
    while (FLASH->SR & FLASH_SR_BSY); // wait till erase complete
    
    FLASH->CR &= ~FLASH_CR_SER; // deactivate erase mode
    
    uart_send_msg("completed\n\r");
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

uint8_t check_crc()
{
    if (*(uint32_t*)CRC_ADDR == 0xFFFFFFFF)
    {
        return 1; // return 0 if crc is not computed
    }

    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN; // enable crc clock
    CRC->CR = CRC_CR_RESET; // reset crc before use

    uint32_t size = *(uint32_t*)APP_END_ADDR;

    for (uint32_t i=FLASH_START_ADDR; i<FLASH_START_ADDR + size; i += 4)
    {
        uint32_t data = *(uint32_t*)i;
        CRC->DR = data;
    }
    
    uint32_t calculatedCRC = CRC->DR;
    uint32_t storedCRC = *(uint32_t*)CRC_ADDR;
    
    return (calculatedCRC == storedCRC); // return true of crc is match
}