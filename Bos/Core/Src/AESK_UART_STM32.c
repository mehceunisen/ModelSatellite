/*
 * AESK_UART_STM32.c
 *
 *  Created on: Nov 28, 2020
 *      Author: basri
 */
#include "AESK_UART_STM32.h"

void Uart_DMA_Receive_Start(UART_HandleTypeDef *huart, USART_Buffer *usart_buffer)
{
	for(uint16_t i = 0; i < sizeof(USART_Buffer); i++)
	{
		((uint8_t*)usart_buffer)[i] = 0;
	}


	 while(HAL_UART_Receive_DMA(huart, usart_buffer->usart_receive_buffer, USART_BUFFER_SIZE) != HAL_OK);
	__HAL_UART_CLEAR_PEFLAG(huart);

}

int16_t read_DMA_Buffer(UART_HandleTypeDef *huart, USART_Buffer *usart_buffer, uint8_t *temp_buf)
{
	int16_t read_byte = 0;
	    read_byte = huart->RxXferSize - huart->hdmarx->Instance->NDTR - usart_buffer->tail;

	    if(read_byte == 0)
	    {
	    	return read_byte;
	    }

	    else
		{
	    	if(read_byte < 0)
	    	{
	    		read_byte = read_byte + huart->RxXferSize;
	    	}

	    	for(uint16_t i = 0; i < read_byte; i++)
	    	{
	    		temp_buf[i] = usart_buffer->usart_receive_buffer[usart_buffer->tail];
	    		usart_buffer->tail++;
				if(usart_buffer->tail >= huart->RxXferSize)
				{
					usart_buffer->tail = 0;
				}
	    	}
	    	return read_byte;
		}
}


void write_DMA_Buffer(UART_HandleTypeDef *huart, uint8_t* temp_buf, size_t size)
{
	while(HAL_UART_Transmit_DMA(huart, temp_buf, (uint16_t)size) != HAL_OK);
}

void send_compro_UART_DMA(UART_HandleTypeDef *huart, AESK_Ring_Buffer *rng_buf)
{
	uint8_t idx = 0;
	uint8_t temp_buf[255] = {0};
	while(rng_buf->remainder_byte)
	{
		Read_Byte_Ring_Buffer(rng_buf, &(temp_buf[idx]));
		idx++;
	}

	write_DMA_Buffer(huart, temp_buf, idx);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_DMA_Tx_Stop(huart);
}

HAL_StatusTypeDef HAL_UART_DMA_Tx_Stop(UART_HandleTypeDef *huart)
{
uint32_t dmarequest = 0x00U;
dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);
if((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
{
CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

/* Abort the UART DMA Tx channel */
if(huart->hdmatx != NULL)
{
  HAL_DMA_Abort(huart->hdmatx);
}
//UART_EndTxTransfer(huart);
	  /* Disable TXEIE and TCIE interrupts */
	CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
	huart->gState = HAL_UART_STATE_READY;

	return HAL_OK;
}
else return HAL_ERROR;

}







