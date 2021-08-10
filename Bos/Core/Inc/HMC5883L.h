/*
 * HMC5883L.h
 *
 *  Created on: 6 May 2021
 *      Author: merty
 */

#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#include "main.h"

#define  Configuration_Register_A  0x00
#define  Configuration_Register_B  0x01
#define  Mode_Register 			   0x02
#define  Data_Output_X_MSB 		   0x03
#define  Data_Output_X_LSB 		   0x04
#define  Data_Output_Z_MSB 		   0x05
#define  Data_Output_Z_LSB 		   0x06
#define  Data_Output_Y_MSB 		   0x07
#define  Data_Output_Y_LSB		   0x08
#define  Status_Register           0x09
#define  Identification_Register_A 0x10
#define  Identification_Register_B 0x11
#define  Identification_Register_C 0x12

I2C_HandleTypeDef hi2c3;
uint8_t HMC5883datas[6];

int16_t HMC5883_Raw_X, HMC5883_Raw_Y, HMC5883_Raw_Z;

void HMC5883Init();
void HMC5883Read(uint8_t rRegister,uint8_t NumberOfBytes);
void HMC5883Write(uint8_t wRegister,uint8_t Value);
void HMC5883ReadValue();

#endif /* INC_HMC5883L_H_ */
