#include <stdio.h>
#include "stm32f401xc.h"

// For GCC-based toolchains (STM32CubeIDE, PlatformIO)
int _write(int fd, char *ptr, int len)
{
    (void)fd;
    for (int i = 0; i < len; i++) 
    {
        // Convert '\n' to "\r\n" for UART terminals
        if (ptr[i] == '\n')
        {
            while (!(USART2->SR & USART_SR_TXE));
            USART2->DR = '\r';
        }

        // Send character
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = ptr[i];
    }
    return len;
}