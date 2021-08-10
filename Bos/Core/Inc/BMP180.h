/*
 * BMP180.h
 *
 *  Created on: 22 May 2021
 *      Author: merty
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32f4xx_hal.h"

typedef unsigned short u_short;
typedef unsigned long u_long;

#define BMP180_I2C_ADDRESS							0xEE
#define BMP180_CALIB_ADDRESS						0xAA
#define BMP180_MEASURE_VALUE_XLSB					0xF8
#define BMP180_MEASURE_VALUE_LSB					0xF7
#define BMP180_MEASURE_VALUE_MSB					0xF6
#define BMP180_CONTROL_REGISTER						0xF4
#define BMP180_SOFT_RESET_REGISTER					0xE0

#define SAMPLE_NUM 1000

typedef struct
{
	uint8_t Over_Sample_Setting;
}BMP_ConfigTypeDef;

enum OVER_SAMPLE_SETTING
{
	OSS_0      = 0x00, //ultra low power mode
	OSS_1      = 0x01, //standard mode
	OSS_2      = 0x02, //high resolution mode
	OSS_3      = 0x03, //ultra high resolution
};

enum
{
	WFIRST_TIME = 0,
	STEP_ONE 	= 1,
	STEP_TWO	= 2,
};

enum CONTROL_REGISTER_VALUE
{
	TEMPERATURE = 0x2E,
	PRESSURE	= 0x34,
};


typedef struct
{
	long temperature;
	long pressure;
}BMP_RawData_Def;

typedef struct
{
	float Temperature;
	long  Pressure;
	long First_Pressure;
	float Altitude;
}BMP;

typedef struct
{
	short	 AC1;
	short    AC2;
	short    AC3;
	u_short  AC4;
	u_short  AC5;
	u_short  AC6;
	short	 B1;
	short	 B2;
	short 	 MB;
	short    MC;
	short    MD;
}BMP_Calib;

BMP bmp180;
BMP_RawData_Def raw;
BMP_Calib cal;

long X1, X2, X3, B3, B5, B6;
u_long B4, B7;

void BMP180_Config(BMP_ConfigTypeDef *config);
void BMP180_Get_Cal();
void ZeroPBarometer();
void BMP180_Set_TempRaw_Data();
void BMP180_Get_TempRaw_Data();
void BMP180_Set_PressRaw_Data();
void BMP180_Get_PressRaw_Data();
void BMP180_Get_True_Data();
void BMP180_Get_Altitude();

#endif /* INC_BMP180_H_ */
