/*
 * systick.c
 *
 *  Created on: May 4, 2021
 *      Author: merty
 */

#include "stdint.h"
#include "systick.h"

uint8_t counter;

void HAL_SYSTICK_Callback(void){
	counter_1ms++;
	if(counter_1ms){
		tt.tasks.task_1000_Hz = 1;
	}

	if(counter_1ms % 4 == 0){
		tt.tasks.task_250_Hz = 1;
	}

	if(counter_1ms % 5 == 0){
		tt.tasks.task_200_Hz = 1;
	}

	if(counter_1ms % 10 == 0){
		tt.tasks.task_100_Hz = 1;
	}

	if(counter_1ms % 40 == 0){
		counter++;
		if(counter == 3){
			tt.tasks.task_75_Hz = 1;
			counter = 0;
		}
	}

	if(counter_1ms % 20 == 0){
		tt.tasks.task_50_Hz = 1;
	}
	if(counter_1ms % 50 == 0){
		tt.tasks.task_20_Hz = 1;
	}
	if(counter_1ms % 100 == 0){
		tt.tasks.task_10_Hz = 1;
	}
	if(counter_1ms % 500 == 0){
		tt.tasks.task_2_Hz = 1;
	}
	if(counter_1ms % 1000 == 0){
		tt.tasks.task_1_Hz = 1;
	}
}
