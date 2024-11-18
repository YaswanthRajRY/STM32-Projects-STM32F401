#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "stm32f401xc.h"

#define SPI_1 (void*)(SPI1_BASE)
#define SPI_2 (void*)(SPI2_BASE)
#define SPI_3 (void*)(SPI3_BASE)

enum operation_mode
{
    SLAVE,
    MASTER
};

enum dataFrameFormat
{
    Bit_8,
    Bit_16
};

enum SPI_mode
{
    MODE_0,         // CPOL->0	CPHA->0	Clock is low when idle, data sampled on rising edge.
    MODE_1,         // CPOL->0	CPHA->1 Clock is low when idle, data sampled on falling edge.
    MODE_2,         // CPOL->1	CPHA->0 Clock is high when idle, data sampled on falling edge.
    MODE_3          // CPOL->1	CPHA->1 Clock is high when idle, data sampled on rising edge.
};

enum Baud_Rate_Prescaler
{
    PRE_2,
    PRE_4,
    PRE_8,
    PRE_16,
    PRE_32,
    PRE_64,
    PRE_128,
    PRE_256
};

typedef struct
{
    uint32_t peripheralClock;
    uint8_t operationMode;
    uint8_t baudRatePrescaler;
    uint8_t SPImode;
    uint8_t TIenable;
    uint8_t dataOrder;
    uint8_t dataFrameFormat;
}SPIconfig_Typedef;

void SPI_init(SPI_TypeDef* SPI, SPIconfig_Typedef* SPIconfig);

#endif