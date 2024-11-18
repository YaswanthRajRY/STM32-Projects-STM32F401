#include "SPI.h"

/*************************************** Setup SPI *******************************************/
void SPI_init(SPI_TypeDef* SPI, SPIconfig_Typedef* SPIconfig)
{
    // Enable SPI peripheral clock
    if ((void*)SPI == (void*)SPI1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    }
    else if ((void*)SPI == (void*)SPI2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    }
    else if ((void*)SPI == (void*)SPI3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    }

    // Set baud rate
    if (SPIconfig->operationMode)                       // if operation mode is master i.e SPIconfig->operationMode = MASTER or 1
    {
        SPI->CR1 |= (SPIconfig->baudRatePrescaler << 3);
    }

    // Data frame format
    if (SPIconfig->dataFrameFormat)
    {
        SPI->CR1 |= SPI_CR1_DFF;                        // 16-bit DFF
    }
    else
    {
        SPI->CR1 &= ~(SPI_CR1_DFF);                     // 8-bit DFF
    }

    // Select CPOL & CPHA
    SPI->CR1 |= SPIconfig->SPImode;

    // Configure data order MSB first or LSB first
    if (SPIconfig->dataOrder)
    {
        SPI->CR1 |= SPI_CR1_LSBFIRST;                   // LSB transmitted first
    }
    else
    {
        SPI->CR1 &= ~(SPI_CR1_LSBFIRST);                // MSB transmitted first
    }

    // Frame format SPI TI mode or SPI Motorola mode
    if (SPIconfig->TIenable)
    {
        SPI->CR2 |= SPI_CR2_FRF;                        // SPI TI mode
    }
    else
    {
        SPI->CR2 &= ~(SPI_CR2_FRF);                     // SPI Motorola mode
    }

    // Enable master
    if (SPIconfig->operationMode)
    {
        SPI->CR1 |= SPI_CR1_MSTR;                       // Enable Master
    }
    else
    {
        SPI->CR1 &= ~(SPI_CR1_MSTR);                    // Enable slave
    }

    // Enable SPI
    SPI->CR1 |= SPI_CR1_SPE;
}