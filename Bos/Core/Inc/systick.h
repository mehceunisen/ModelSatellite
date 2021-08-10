/*
 * systick.h
 *
 *  Created on: May 4, 2021
 *      Author: merty
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

typedef union
{
	struct
	{
		uint16_t task_1000_Hz : 1;
		uint16_t task_250_Hz  : 1;
		uint16_t task_200_Hz  : 1;
		uint16_t task_100_Hz  : 1;
		uint16_t task_75_Hz   : 1;
		uint16_t task_50_Hz   : 1;
		uint16_t task_20_Hz   : 1;
		uint16_t task_10_Hz   : 1;
		uint16_t task_2_Hz    : 1;
		uint16_t task_1_Hz    : 1;
		uint16_t reserved     : 6;
	}tasks;
	uint16_t all;
}time_task_t;
uint16_t counter_1ms;
time_task_t tt;

#endif /* INC_SYSTICK_H_ */




