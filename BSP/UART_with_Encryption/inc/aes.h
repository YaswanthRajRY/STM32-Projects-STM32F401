#ifndef AES_H
#define AES_H

#include "stm32f401xc.h"

#define KEY "0123456789012345"

void aes_init(void);
uint8_t* aes_encryption(uint8_t* plain_text);
uint8_t* aes_decryption(uint8_t* encrypted_text);

#endif