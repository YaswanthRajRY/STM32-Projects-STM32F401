#include "ringBuffer.h"

/****************************** Buffer initialization *******************************/
void ringBuffer_init(ringBuffer_Typedef* buff)
{
    buff->head = 0;
    buff->tail = 0;
}

/********************************** Buffer flags *************************************/
uint8_t ringBuffer_isFull(ringBuffer_Typedef* buff)
{
    uint8_t nextHead = (buff->head + 1) % MAX_SIZE_RING_BUFFER;
    return (nextHead == buff->tail);
}

uint8_t ringBuffer_isEmpty(ringBuffer_Typedef* buff)
{
    return (buff->head == buff->tail);
}

/****************************** Buffer read and write *******************************/
void ringBuffer_Write(ringBuffer_Typedef* buff, uint8_t data)
{
    if (!(ringBuffer_isFull(buff)))
    {
        buff->buffer[buff->head] = data;
        buff->head = ((buff->head + 1) % MAX_SIZE_RING_BUFFER);
    }
}

uint8_t ringBuffer_Read(ringBuffer_Typedef* buff)
{
    if (!(ringBuffer_isEmpty(buff)))
    {
        uint8_t data = buff->buffer[buff->tail];
        buff->tail = ((buff->tail + 1) % MAX_SIZE_RING_BUFFER);
        return data;
    }
    
    return '\0';
}