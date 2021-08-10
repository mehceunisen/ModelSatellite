/*
 * AESK_GENERAL.c
 *
 *  Created on: 17 Oca 2021
 *      Author: Ahmet
 */

#include "AESK_GENERAL.h"
extern uint8_t yakup_flag_zpc;

float AESK_Map(int32_t incoming_value, int32_t fromMin, int32_t fromMax, float toMin, float toMax)
{
    return ((((incoming_value - fromMin)*( toMax - toMin))/(fromMax - fromMin)) + toMin);
}

void AESK_Iar_Filter(float* filtering_data, float filtering_data_coeffecient, float fresh_data, float fresh_data_coeffecient)
{
	*filtering_data = *filtering_data * fresh_data_coeffecient + fresh_data * fresh_data_coeffecient;
}
void AESK_Median_Filter(float filtering_data, float* filtered_data, uint8_t sample_count)
{
	static uint8_t counter = 0;
	static float sum = 0;
	counter++;
	if(counter == sample_count)
	{
		*filtered_data = sum / sample_count;
		counter = 0;
		sum = 0;
		//yakup_flag_zpc = 1;
	}
	sum += filtering_data;

}
