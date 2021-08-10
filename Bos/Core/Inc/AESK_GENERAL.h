/*
 * AESK_GENEREAL.h
 *
 *  Created on: 16 Oca 2021
 *      Author: Ahmet
 */

#ifndef AESK_GENEREAL_H_
#define AESK_GENEREAL_H_

#include "main.h"


//Change this according to your i2c peripheral selection!
#define I2C_TYPEDEF				&hi2c1
#define PI 						3.14159265359
#define GRAVITATIONAL_CONSTANT 	9.80665

typedef enum
{
  AESK_OK       = 0x00U,
  AESK_COM_ERROR    = 0x01U,
} AESK_StatusTypeDef;

AESK_StatusTypeDef aesk_status;

float AESK_Map(int32_t incoming_value, int32_t fromMin, int32_t fromMax, float toMin, float toMax);
void AESK_Iar_Filter(float* filtered_data, float filtered_data_coeffecient, float fresh_data, float fresh_data_coeffecient);
void AESK_Median_Filter(float filtering_data, float* filtered_data, uint8_t sample_count);


#endif /* AESK_GENEREAL_H_ */
