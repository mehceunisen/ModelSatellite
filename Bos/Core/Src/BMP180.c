/*
 * BMP180.c
 *
 *  Created on: 22 May 2021
 *      Author: merty
 */

#include "BMP180.h"
#include "math.h"
extern I2C_HandleTypeDef hi2c1;
uint8_t oss;

void BMP180_Config(BMP_ConfigTypeDef *config)
{
	oss = config->Over_Sample_Setting;
}

void BMP180_Get_Cal()
{
	uint8_t Cal_Data[22] = {0};
	HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDRESS, BMP180_CALIB_ADDRESS, 1, Cal_Data, 22, HAL_MAX_DELAY);
	cal.AC1 = ((Cal_Data[0] << 8) | Cal_Data[1]);
	cal.AC2 = ((Cal_Data[2] << 8) | Cal_Data[3]);
	cal.AC3 = ((Cal_Data[4] << 8) | Cal_Data[5]);
	cal.AC4 = ((Cal_Data[6] << 8) | Cal_Data[7]);
	cal.AC5 = ((Cal_Data[8] << 8) | Cal_Data[9]);
	cal.AC6 = ((Cal_Data[10] << 8) | Cal_Data[11]);
	cal.B1  = ((Cal_Data[12] << 8) | Cal_Data[13]);
	cal.B2  = ((Cal_Data[14] << 8) | Cal_Data[15]);
	cal.MB  = ((Cal_Data[16] << 8) | Cal_Data[17]);
	cal.MC  = ((Cal_Data[18] << 8) | Cal_Data[19]);
	cal.MD  = ((Cal_Data[20] << 8) | Cal_Data[21]);
}

void ZeroPBarometer(){
	uint64_t zeroP = 0;
	for(uint16_t i=0;i<SAMPLE_NUM;i++){
		BMP180_Set_PressRaw_Data();
		HAL_Delay(5);
		BMP180_Get_PressRaw_Data();
		zeroP += raw.pressure;
	}
	zeroP /= SAMPLE_NUM;
	BMP180_Set_TempRaw_Data();
	HAL_Delay(5);
	BMP180_Get_TempRaw_Data();
	X1 = ((raw.temperature-cal.AC6) * (cal.AC5/(pow(2,15))));
	X2 = ((cal.MC * (pow(2,11))) / (X1 + cal.MD));
	B5 = X1 + X2;
	bmp180.Temperature = ((B5+8) / (pow(2,4))) / 10.0;
	B6 = B5 - 4000;
	X1 = (cal.B2 * (pow(B6,2) / (pow(2,12)))) / (pow(2,11));
	X2 = cal.AC2 * B6 / (pow(2,11));
	X3 = X1 + X2;
	B3 = (((cal.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = cal.AC3 * B6 / pow(2,13);
	X2 = (cal.B1 * (pow(B6,2) / (pow(2,12)))) / (pow(2,16));
	X3 = ((X1 + X2) + 2) / pow(2,2);
	B4 = cal.AC4 * (unsigned long)(X3 + 32768) / (pow(2,15));
	B7 = ((unsigned long)zeroP - B3) * (50000>>oss);
	if (B7 < 0x80000000) bmp180.First_Pressure = (B7*2)/B4;
	else bmp180.First_Pressure  = (B7/B4)*2;
	X1 = (bmp180.First_Pressure  / (pow(2,8))) * (bmp180.First_Pressure  / (pow(2,8)));
	X1 = (X1 * 3038) / (pow(2,16));
	X2 = (-7357 * bmp180.First_Pressure ) / (pow(2,16));
}

void BMP180_Set_TempRaw_Data()
{
	uint8_t data = TEMPERATURE;
	HAL_I2C_Mem_Write(&hi2c1, BMP180_I2C_ADDRESS, BMP180_CONTROL_REGISTER, 1, &data, 1, HAL_MAX_DELAY);
}

void BMP180_Get_TempRaw_Data()
{
	uint8_t Temp_Raw[2];
	HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDRESS, BMP180_MEASURE_VALUE_MSB, 1, Temp_Raw, 2, HAL_MAX_DELAY);
	raw.temperature = (Temp_Raw[0]<<8) | Temp_Raw[1];
}

void BMP180_Get_True_Data()
{
	X1 = ((raw.temperature-cal.AC6) * (cal.AC5/(pow(2,15))));
	X2 = ((cal.MC * (pow(2,11))) / (X1 + cal.MD));
	B5 = X1 + X2;
	bmp180.Temperature = ((B5+8) / (pow(2,4))) / 10.0;
	B6 = B5 - 4000;
	X1 = (cal.B2 * (pow(B6,2) / (pow(2,12)))) / (pow(2,11));
	X2 = cal.AC2 * B6 / (pow(2,11));
	X3 = X1 + X2;
	B3 = (((cal.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = cal.AC3 * B6 / pow(2,13);
	X2 = (cal.B1 * (pow(B6,2) / (pow(2,12)))) / (pow(2,16));
	X3 = ((X1 + X2) + 2) / pow(2,2);
	B4 = cal.AC4 * (unsigned long)(X3 + 32768) / (pow(2,15));
	B7 = ((unsigned long)raw.pressure - B3) * (50000>>oss);
	if (B7 < 0x80000000) bmp180.Pressure = (B7*2)/B4;
	else bmp180.Pressure  = (B7/B4)*2;
	X1 = (bmp180.Pressure  / (pow(2,8))) * (bmp180.Pressure  / (pow(2,8)));
	X1 = (X1 * 3038) / (pow(2,16));
	X2 = (-7357 * bmp180.Pressure ) / (pow(2,16));
	BMP180_Get_Altitude();
}

void BMP180_Set_PressRaw_Data()
{
	uint8_t data = PRESSURE + (oss << 6);
	HAL_I2C_Mem_Write(&hi2c1, BMP180_I2C_ADDRESS, BMP180_CONTROL_REGISTER, 1, &data, 1, HAL_MAX_DELAY);
}

void BMP180_Get_PressRaw_Data()
{
	uint8_t Press_Raw[3] = {0};
	HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDRESS, BMP180_MEASURE_VALUE_MSB, 1, Press_Raw, 2, HAL_MAX_DELAY);
	raw.pressure = (((Press_Raw[0]<<16)+(Press_Raw[1]<<8)+Press_Raw[2]) >> (8-oss));
}

void BMP180_Get_Altitude()
{
	bmp180.Altitude = 44330*(1-(pow(((float)bmp180.Pressure/(float)bmp180.First_Pressure), 0.19029495718)));
}


