#include "main.h"

#define SysCoreClk 42000000

UART_Typedef UART1_config = {
        .baudRate = 9600,               // baud rate
        .peripheralClock = SysCoreClk,  // peripheral clock
        .mode = UART_TX,                // UART communication mode
        .ParityEnable = 0,              // enable parity
        .Parity = 0,                    // even parity
        .NoStopBit = 1                  // stop bit
};

int main(void)
{
    SystemInit();			            // Initialize system
    
    UART_init(UART1, &UART1_config);    // setup UART 1

    while (1)
    {
        
    }
}
