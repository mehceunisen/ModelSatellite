/*
 * LM35.c
 *
 *  Created on: Jun 28, 2021
 *      Author: merty
 */

#include <main.h>
#include <LM35.h>
#include <AESK_GENERAL.h>

float filtering_data_coeffecient = 0.50;
float fresh_data_coeffecient = 0.50;

uint16_t Get_Temperature(ADC_HandleTypeDef* hadc)
{
	float mV = 0;	uint16_t temperature = 0;
	mV = ((float)HAL_ADC_GetValue(hadc) / ADC_RES) * VREFIN;
	temperature = (uint16_t)mV / 10;
	return temperature;
}
