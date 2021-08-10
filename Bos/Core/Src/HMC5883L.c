/*
 * HMC5883L.c
 *
 *  Created on: 6 May 2021
 *      Author: merty
 */
#include "HMC5883L.h"
#include "main.h"

void HMC5883LWrite(uint8_t wRegister,uint8_t Value)
{
	uint8_t data[2];
	data[0]         = wRegister;
	data[1]         = Value;
	//Slave Adresi HAL_I2C_Master_Transmit(&hi2c3, ManyometerSlaveAdressWrite, data, 2, 10);

}
void HMC5883LRead(uint8_t rRegister,uint8_t NumberOfBytes)
{
	//Slave Adresi HAL_I2C_Mem_Read(&hi2c3, ManyometerSlaveAdressRead, rRegister, 1, HMC5883datas, NumberOfBytes, 10);

}
void HMC5883LInit()
{
	HAL_Delay(10);
	HMC5883LWrite(Configuration_Register_A,0x78);
	HMC5883LWrite(Configuration_Register_B,0x96); //Gain Sorulacak
}
void HMC5883LReadValue()
{
	HMC5883LRead(0x68, 6);
	HMC5883_Raw_X = (HMC5883datas[0] << 8) | HMC5883datas[1];
	HMC5883_Raw_Y = (HMC5883datas[2] << 8) | HMC5883datas[3];
	HMC5883_Raw_Z = (HMC5883datas[4] << 8) | HMC5883datas[5];
}


