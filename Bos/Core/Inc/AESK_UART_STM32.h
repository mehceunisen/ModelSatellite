/*
 * AESK_UART_STM32.h
 *
 *  Created on: Nov 28, 2020
 *      Author: basri
 */

#ifndef INC_AESK_UART_STM32_H_
#define INC_AESK_UART_STM32_H_
#include "main.h"
#include "AESK_Ring_Buffer.h"

#define USART_BUFFER_SIZE 		1024
typedef struct
{
	uint8_t usart_receive_buffer[USART_BUFFER_SIZE];
	uint16_t tail;
}USART_Buffer;

USART_Buffer uart_buf;
USART_Buffer esp32_buf;

void Uart_DMA_Receive_Start(UART_HandleTypeDef *huart, USART_Buffer *usart_buffer);

int16_t read_DMA_Buffer(UART_HandleTypeDef *huart, USART_Buffer *usart_buffer, uint8_t *temp_buf);

void send_compro_UART_DMA(UART_HandleTypeDef *huart, AESK_Ring_Buffer *rng_buf);

void write_DMA_Buffer(UART_HandleTypeDef *huart, uint8_t* temp_buf, size_t size);

HAL_StatusTypeDef HAL_UART_DMA_Tx_Stop(UART_HandleTypeDef *huart);

#endif /* INC_AESK_UART_STM32_H_ */
