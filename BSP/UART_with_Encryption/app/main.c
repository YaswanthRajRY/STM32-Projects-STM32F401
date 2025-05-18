#include "main.h"

void uart1_aes_encrypt(uint8_t* dt);
void uart1_aes_decrypt(uint8_t* dt);
uint8_t uart1_rx(void);

uint8_t a[16]; 

int main()
{
    SystemInit();
    aes_init();
    uart1_init(115200);
    uart2_init(115200);
    //uart6_init(115200);
    
    printf("******** UART with AES encryption ********\n");

    uint8_t str[16] = "Hello, World! :)";
    printf("Original: %s\n", str);
    uart1_aes_encrypt(str);

    uart1_aes_decrypt(a);

    while (1)
    {

    }
}

void uart1_aes_encrypt(uint8_t* dt)
{
    uint8_t* tx = aes_encryption(dt);
    printf("Encrypted: ");
    for(int i=0; i<16; i++)
    {
        printf("%X", tx[i]);
    }
    printf("\n");

    for (int i=0; i<16; i++)
    {
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = (uint8_t)tx[i];

        while (!(USART1->SR & USART_SR_RXNE));
        a[i] = USART1->DR;
    }
}

void uart1_aes_decrypt(uint8_t* dt)
{
    uint8_t* rx = aes_decryption(dt);
    printf("Decrypted: ");
    for(int i=0; i<16; i++)
    {
        printf("%c", rx[i]);
    }
    printf("\n");
}

uint8_t uart1_rx()
{
    while(!(USART1->SR & USART_SR_RXNE));
    return USART1->DR;
}
