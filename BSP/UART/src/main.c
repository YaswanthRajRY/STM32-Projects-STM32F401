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

UART_Typedef UART6_config = {
        .baudRate = 9600,               // baud rate
        .peripheralClock = SysCoreClk,  // peripheral clock
        .mode = UART_TX_RX,             // UART communication mode
        .ParityEnable = 0,              // enable parity
        .Parity = 0,                    // even parity
        .NoStopBit = 1                  // stop bit
};

ringBuffer_Typedef UART6_Buff;

int main(void)
{
    SystemInit();			            // Initialize system
    
    UART_init(UART1, &UART1_config);    // setup UART 1
    UART_init(UART6, &UART6_config);    // setup UART 6

    ringBuffer_init(&UART6_Buff);       // setup ring buffer for UART 6

    while (1)
    {
        UART_EnableInterrupts_Rx(UART6);     // enable interrupts to receive incoming data
        char* s = "Hello World from UART 1 :)\n\r";
        while (*s)
        {
            UART_Write(UART1, *s);          // write data into buffer 
            s++;
        }

        UART_EnableInterrupts_Tx(UART6);    // enable interrupts to transmit data
      
        Delay_ms(1000);                     // 1sec delay
    }
}
