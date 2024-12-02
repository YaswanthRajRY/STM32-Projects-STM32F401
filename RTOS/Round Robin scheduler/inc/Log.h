#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include "stm32f401xc.h"

#define PERIPHERAL_CLK 42000000     // pheripheral clock

/*
    Initialize UART 1
*/
void Log_init(void);

/*
    Transmit string 
*/
void Log_s(char* data_Tx);

/*
    Transmit char
*/
void Log_c(uint8_t data_Tx);

/*
    Transmit integer
*/
void Log_i(uint32_t data_Tx);

/*
    Transmit pointers
*/
void Log_p(uint32_t* data_Tx);

#endif