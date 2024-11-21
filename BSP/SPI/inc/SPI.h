#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "stm32f401xc.h"
//#include "ringringBuffer.h"

#define SPI_BUFFER_SIZE 64

/*
    macros contains SPI address
*/
#define SPI_1 (void*)(SPI1_BASE)
#define SPI_2 (void*)(SPI2_BASE)
#define SPI_3 (void*)(SPI3_BASE)

/*
    set value as 1 to enable interrupt
*/
#define SPI1_INTERRUPT_ENABLE 0
#define SPI2_INTERRUPT_ENABLE 0
#define SPI3_INTERRUPT_ENABLE 0

/*
    use SPIx_Buff to create ring buffer
    ex: (in main code)
    ringBuffer_Typedef SPI1_Buff;
*/
#if SPI1_INTERRUPT_ENABLE
extern ringBuffer_Typedef SPI1_Buff;
#endif
#if SPI2_INTERRUPT_ENABLE
extern ringBuffer_Typedef SPI2_Buff;
#endif
#if SPI3_INTERRUPT_ENABLE
extern ringBuffer_Typedef SPI3_Buff;
#endif


/*
    use declared enum to select SPI operation mode
*/
enum operation_mode
{
    SLAVE,
    MASTER
};

/*
    use declared enum to select SPI data frame format
*/
enum dataFrameFormat
{
    Bit_8,
    Bit_16
};

/*
    use declared enum to select SPI mode
*/
enum SPI_mode
{
    MODE_0,         // CPOL->0	CPHA->0	Clock is low when idle, data sampled on rising edge.
    MODE_1,         // CPOL->0	CPHA->1 Clock is low when idle, data sampled on falling edge.
    MODE_2,         // CPOL->1	CPHA->0 Clock is high when idle, data sampled on falling edge.
    MODE_3          // CPOL->1	CPHA->1 Clock is high when idle, data sampled on rising edge.
};

/*
    use declared enum to select SPI baud rate prescale
*/
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

/*
    use declared struct to configure SPI function
*/
typedef struct
{
    uint8_t operationMode;
    uint8_t baudRatePrescaler;
    uint8_t SPImode;
    uint8_t TIenable;
    uint8_t dataOrder;
    uint8_t dataFrameFormat;
    uint8_t softwareNSS;
    uint8_t NSSactiveHigh;
}SPIconfig_Typedef;

/*
    function declaration
*/
void SPI_init(SPI_TypeDef* SPI, SPIconfig_Typedef* SPIconfig);
void SPI_Enable(SPI_TypeDef* SPI);
void SPI_Disable(SPI_TypeDef* SPI);
void SPI_Write(SPI_TypeDef* SPI, uint16_t data);
uint16_t SPI_Read(SPI_TypeDef* SPI);

#endif