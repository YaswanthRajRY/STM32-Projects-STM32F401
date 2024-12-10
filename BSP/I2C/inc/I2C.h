#ifndef BSP_I2C_H_
#define BSP_I2C_H_

#include "stm32f401xc.h"

// I2C Peripheral Base Addresses
#define I2C1_BASE_ADDR (void*)I2C1_BASE
#define I2C2_BASE_ADDR (void*)I2C2_BASE
#define I2C3_BASE_ADDR (void*)I2C3_BASE

// I2C Configuration Structure
typedef struct
{
    uint32_t clockSpeed;       // I2C Clock Speed in Hz
    uint8_t addressingMode;    // 0 for 7-bit, 1 for 10-bit addressing
    uint8_t ownAddress;        // Device Own Address (if required)
} I2C_Config_Typedef;

// Function Prototypes
void I2C_Init(I2C_TypeDef* I2Cx, I2C_Config_Typedef* config);
void I2C_Start(I2C_TypeDef* I2Cx, uint8_t slaveAddr, uint8_t isRead);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_Read(I2C_TypeDef* I2Cx, uint8_t ack);
void I2C_Stop(I2C_TypeDef* I2Cx);

#endif
