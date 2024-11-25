#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

/*
    Define your buffer size below
*/
#define MAX_SIZE_RING_BUFFER 64

/*
    use below struct and create object for new buffer
    ex: ringBuffer_Typedef objName;
*/
typedef struct
{
    uint8_t buffer[MAX_SIZE_RING_BUFFER];
    uint8_t head;
    uint8_t tail;
}ringBuffer_Typedef;

/*
    function declaration
*/
void ringBuffer_init(ringBuffer_Typedef* buff);
void ringBuffer_Write(ringBuffer_Typedef* buff, uint8_t data);

uint8_t ringBuffer_Read(ringBuffer_Typedef* buff);
uint8_t ringBuffer_isFull(ringBuffer_Typedef* buff);
uint8_t ringBuffer_isEmpty(ringBuffer_Typedef* buff);

#endif