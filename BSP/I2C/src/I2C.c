#include "I2C.h"

/************************************** I2C Initialization ***************************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_Config_Typedef* config)
{
    // Enable Clock for I2C Peripheral
    if (I2Cx == I2C1)
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    else if (I2Cx == I2C2)
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    else if (I2Cx == I2C3)
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

    // Peripheral Reset
    I2Cx->CR1 |= I2C_CR1_SWRST;                         // Set the SWRST bit to reset the I2C peripheral
    I2Cx->CR1 &= ~I2C_CR1_SWRST;                        // Clear the SWRST bit to release the reset

    // Configure Clock Control Register
    uint32_t pclk1 = 42000000;                          // Assuming APB1 clock is 42 MHz
    I2Cx->CR2 = (pclk1 / 1000000) & I2C_CR2_FREQ;       // Set peripheral clock frequency

    // Set Clock Speed
    I2Cx->CCR = (pclk1 / (config->clockSpeed * 2)) & 0xFFF;

    // Configure TRISE Register
    I2Cx->TRISE = ((pclk1 / 1000000) + 1) & 0x3F;

    // Configure Addressing Mode
    if (config->addressingMode == 1)
        I2Cx->OAR1 |= I2C_OAR1_ADDMODE;                 // 10-bit addressing
    I2Cx->OAR1 |= config->ownAddress << 1;              // Set own address

    // Enable I2C Peripheral
    I2Cx->CR1 |= I2C_CR1_PE;
}

/************************************** I2C Start Condition ***************************************/
void I2C_Start(I2C_TypeDef* I2Cx, uint8_t slaveAddr, uint8_t isRead)
{
    // Generate Start Condition
    I2Cx->CR1 |= I2C_CR1_START;

    // Wait for Start Condition to be Generated
    while (!(I2Cx->SR1 & I2C_SR1_SB));

    // Send Slave Address + R/W bit
    I2Cx->DR = (slaveAddr << 1) | isRead;

    // Wait for Address to be Sent
    while (!(I2Cx->SR1 & I2C_SR1_ADDR));
    (void)I2Cx->SR2;                                    // Clear ADDR Flag by Reading SR1 and SR2
}

/************************************** I2C Write Data *******************************************/
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t data)
{
    // Wait until Data Register is Empty
    while (!(I2Cx->SR1 & I2C_SR1_TXE));

    // Write Data to Data Register
    I2Cx->DR = data;

    // Wait until Data is Transmitted
    while (!(I2Cx->SR1 & I2C_SR1_BTF));
}

/************************************** I2C Read Data ********************************************/
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t ack)
{
    // Configure ACK Control
    if (ack)
        I2Cx->CR1 |= I2C_CR1_ACK;
    else
        I2Cx->CR1 &= ~I2C_CR1_ACK;

    // Wait until Data is Received
    while (!(I2Cx->SR1 & I2C_SR1_RXNE));

    // Return Data from Data Register
    return I2Cx->DR;
}

/************************************** I2C Stop Condition ***************************************/
void I2C_Stop(I2C_TypeDef* I2Cx)
{
    // Generate Stop Condition
    I2Cx->CR1 |= I2C_CR1_STOP;
}