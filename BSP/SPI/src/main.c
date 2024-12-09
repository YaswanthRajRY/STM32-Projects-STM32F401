#include "main.h"

#define SysCoreClk 42000000

SPIconfig_Typedef SPI1_config = {
    .baudRatePrescaler = PRE_256,       // BR 256
    .operationMode = MASTER,            // SPI as master
    .dataOrder = 0,                     // MSB first
    .dataFrameFormat = Bit_8,           // 8 bit data frame
    .SPImode = MODE_2,                  // CPOL->1 & CPHA->0
    .TIenable = 0,                      // Motorola mode
    .softwareNSS = 0                    // Software NSS diabled
};

SPIconfig_Typedef SPI2_config = {
    .operationMode = SLAVE,             // SPI as slave
    .dataOrder = 0,                     // MSB first
    .dataFrameFormat = Bit_8,           // 8 bit data frame
    .SPImode = MODE_2,                  // CPOL->1 & CPHA->0
    .TIenable = 0,                      // Motorola mode
    .softwareNSS = 0                    // Software NSS diabled
};

/*
UART_Typedef UART1_config = {
    .baudRate = 9600,
    .peripheralClock = SysCoreClk,
    .mode = UART_TX,
    .NoStopBit = 1
};
    Use UART 1 to view received SPI data in serial monitor */

int main(void)
{
    SystemInit();

    //UART_init(UART1, &UART1_config);            // UART to see the receive data in serial monitor

    SPI_init(SPI_1, &SPI1_config);              // Setup SPI 1
    SPI_init(SPI_2, &SPI2_config);              // Setup SPI 2

    uint8_t data;

    while (1)
    {
        SPI_Enable(SPI_1);                      // enable SPI
        SPI_Enable(SPI_2);

        char* str = "Hello World\n\r";
        while (*str)
        {
            SPI_Write(SPI_1, *str++);           // transmit data from SPI 1
            SPI_Read(SPI_1);                    // Read DR to clear over right flag being set
            if (GPIOB->IDR & GPIO_IDR_ID12)     // if NSS goes Low to High (i.e) Master deselects the slave
            {
                break;
            }
            SPI_Write(SPI_2, 0xFF);             // Send dummy data from MISO salve to master
            data = SPI_Read(SPI_2);             // receive data from SPI 2
            //UART_Write(UART1, data);            // UART to see the receive data in serial monitor
        }

        SPI_Disable(SPI_1);                     // disable SPI
        SPI_Disable(SPI_2);

        Delay_ms(1000);                         // 1s delay
    }
}