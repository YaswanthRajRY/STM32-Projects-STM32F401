#include "SPI.h"

/*************************************** Setup SPI *******************************************/
void SPI_init(SPI_TypeDef* SPI, SPIconfig_Typedef* SPIconfig)
{
    // Enable SPI peripheral clock
    if ((void*)SPI == (void*)SPI1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;         // SPI 1 clock enable
    }
    else if ((void*)SPI == (void*)SPI2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;         // SPI 2 clock enable
    }
    else if ((void*)SPI == (void*)SPI3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;         // SPI 3 clock enable
    }

    SPI->CR1 &= ~SPI_CR1_SPE;

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

    // Config NSS
    if (SPIconfig->softwareNSS)
    {
        SPI->CR1 |= (SPIconfig->softwareNSS << SPI_CR1_SSM_Pos);    // SSM
        SPI->CR1 |= (SPIconfig->NSSactiveHigh << SPI_CR1_SSI_Pos);  // SSI
    }
    else
    {
        SPI->CR2 |= SPI_CR2_SSOE;                       // In master mode, NSS is output enabled
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
}

/************************************ Enable/Disable DPI **************************************/
void SPI_Enable(SPI_TypeDef* SPI)
{
    SPI->CR1 |= SPI_CR1_SPE;
}
void SPI_Disable(SPI_TypeDef* SPI)
{
    SPI->CR1 &= ~SPI_CR1_SPE;
}

/************************************* Transmit data *****************************************/
void SPI_Write(SPI_TypeDef* SPI, uint16_t data)
{
    while (!(SPI->SR & SPI_SR_TXE));        // wait till data in transmit buffer is empty
    SPI->DR = data;                         // write data into data register

    while (SPI->SR & SPI_SR_BSY);           // wait till communication is not busy
}

/************************************* Receive data ******************************************/
uint16_t SPI_Read(SPI_TypeDef* SPI)
{
    while (!(SPI->SR & SPI_SR_TXE));        // wait till data in transmit buffer is empty
    SPI->DR = 0xFF;                         // in master mode MISO send dummy data. in slave mode triggers reading the incoming data

    while (!(SPI->SR & SPI_SR_RXNE));       // wait till data in receive buffer is empty
    return SPI->DR;                         // return received data
}