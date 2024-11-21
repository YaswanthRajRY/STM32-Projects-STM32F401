#include "SPI.h"

/*************************************** Setup SPI *******************************************/
void SPI_init(SPI_TypeDef* SPI, SPIconfig_Typedef* SPIconfig)
{
    // Enable SPI peripheral clock
    if ((void*)SPI == (void*)SPI1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;         // SPI 1 clock enable
        #if SPI1_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(SPI1_IRQn);
        __enable_irq();
        #endif
    }
    else if ((void*)SPI == (void*)SPI2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;         // SPI 2 clock enable
        #if SPI2_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(SPI2_IRQn);
        __enable_irq();
        #endif
    }
    else if ((void*)SPI == (void*)SPI3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;         // SPI 3 clock enable
        #if SPI3_INTERRUPT_ENABLE
        __disable_irq();
        NVIC_EnableIRQ(SPI3_IRQn);
        __enable_irq();
        #endif
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

    // Enable Interrupt for Tx and Rx
    #if SPI1_INTERRUPT_ENABLE || SPI2_INTERRUPT_ENABLE || SPI3_INTERRUPT_ENABLE
    SPI->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;
    #endif

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
    while (SPI->SR & SPI_SR_BSY);           // wait till communication is not busy

    SPI->CR1 &= ~SPI_CR1_SPE;
}

/************************************* Transmit data *****************************************/
void SPI_Write(SPI_TypeDef* SPI, uint16_t data)
{
    while (!(SPI->SR & SPI_SR_TXE));        // wait till data in transmit buffer is empty
    SPI->DR = data;                         // write data into data register
}

/************************************* Receive data ******************************************/
uint16_t SPI_Read(SPI_TypeDef* SPI)
{
    while (!(SPI->SR & SPI_SR_RXNE));       // wait till data in receive buffer is empty
    return SPI->DR;                         // return received data
}

/****************************************** ISR **********************************************/
#if SPI1_INTERRUPT_ENABLE
void SPI_IRQHandler(void) 
{
    if (SPI1->SR & SPI_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&SPI1_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&SPI1_Buff);
            SPI1->DR = Tx_Data;
        }
        else
        {
            SPI1->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    if (SPI1->SR & SPI_SR_RXNE)
    {
        uint8_t Rx_data = SPI1->DR;
        if (!(ringBuffer_isFull(&SPI1_Buff)))
        {
            ringBuffer_Write(&SPI1_Buff, Rx_data);
        }
        else
        {
            SPI1->CR2 &= ~SPI_CR2_RXNEIE;
        }
    }

    // Check for SPI errors (Optional)
    if (SPI1->SR & SPI_SR_OVR)
    {
        uint8_t data = SPI1->DR;
        data = SPI1->SR;
        (void)data;
    }
}
#endif

#if SPI2_INTERRUPT_ENABLE
void SPI_IRQHandler(void) 
{
    if (SPI2->SR & SPI_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&SPI2_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&SPI2_Buff);
            SPI2->DR = Tx_Data;
        }
        else
        {
            SPI2->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    if (SPI2->SR & SPI_SR_RXNE)
    {
        uint8_t Rx_data = SPI2->DR;
        if (!(ringBuffer_isFull(&SPI2_Buff)))
        {
            ringBuffer_Write(&SPI2_Buff, Rx_data);
        }
        else
        {
            SPI2->CR2 &= ~SPI_CR2_RXNEIE;
        }
    }

    // Check for SPI errors (Optional)
    if (SPI2->SR & SPI_SR_OVR)
    {
        uint8_t data = SPI2->DR;
        data = SPI2->SR;
        (void)data;
    }
}
#endif


#if SPI3_INTERRUPT_ENABLE
void SPI_IRQHandler(void) 
{
    if (SPI3->SR & SPI_SR_TXE)
    {
        if (!(ringBuffer_isEmpty(&SPI3_Buff)))
        {
            uint8_t Tx_Data = ringBuffer_Read(&SPI3_Buff);
            SPI3->DR = Tx_Data;
        }
        else
        {
            SPI3->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    if (SPI3->SR & SPI_SR_RXNE)
    {
        uint8_t Rx_data = SPI3->DR;
        if (!(ringBuffer_isFull(&SPI3_Buff)))
        {
            ringBuffer_Write(&SPI3_Buff, Rx_data);
        }
        else
        {
            SPI3->CR2 &= ~SPI_CR2_RXNEIE;
        }
    }

    // Check for SPI errors (Optional)
    if (SPI3->SR & SPI_SR_OVR)
    {
        uint8_t data = SPI3->DR;
        data = SPI3->SR;
        (void)data;
    }
}
#endif