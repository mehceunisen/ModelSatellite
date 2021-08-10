/*
 * AESK_Ring_Buffer.h
 *
 *  Created on: Nov 28, 2020
 *      Author: basri
 */

#ifndef INC_AESK_RING_BUFFER_H_
#define INC_AESK_RING_BUFFER_H_
#include <stdint.h>
#include <stddef.h>
#define RING_BUFFER_SIZE  2048

typedef struct
{
	uint16_t tail;
	uint16_t head;
	uint8_t ring_buffer[RING_BUFFER_SIZE];
	uint16_t remainder_byte;

}AESK_Ring_Buffer;

void Ring_Buffer_Init(AESK_Ring_Buffer* rng_buf);
void Write_Data_Ring_Buffer(AESK_Ring_Buffer* rng_buf, const uint8_t* tmp_buf, size_t size);
int16_t Read_Byte_Ring_Buffer(AESK_Ring_Buffer* rng_buf, uint8_t* tmp_buf);
int16_t Read_All_Buffer(AESK_Ring_Buffer* rng_buf, uint8_t* tmp_buf);


#endif /* INC_AESK_RING_BUFFER_H_ */
