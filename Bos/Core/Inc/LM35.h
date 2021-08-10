/*
 * LM35.h
 *
 *  Created on: Jun 28, 2021
 *      Author: merty
 */

#ifndef INC_LM35_H_
#define INC_LM35_H_

#define mV_to_Celcius 10
#define VREFIN		  2.975
#define ADC_RES		  4096


uint16_t Get_Temperature();
#endif /* INC_LM35_H_ */
