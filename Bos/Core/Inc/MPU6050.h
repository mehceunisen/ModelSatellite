/*
 * MPU6050.h
 *
 *  Created on: 17 May 2021
 *      Author: merty
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_


#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define WHO_AM_I_REG		0x75
#define MPU_ADDR			0x68
#define PWR_MAGT_1_REG		0x6B
#define CONFIG_REG			0x1A
#define GYRO_CONFIG_REG		0x1B
#define ACCEL_CONFIG_REG	0x1C
#define SMPLRT_DIV_REG		0x19
#define INT_STATUS_REG		0x3A
#define ACCEL_XOUT_H_REG	0x3B
#define TEMP_OUT_H_REG		0x41
#define GYRO_XOUT_H_REG		0x43
#define FIFO_EN_REG 		0x23
#define INT_ENABLE_REG 		0x38
#define I2CMACO_REG 		0x23
#define USER_CNT_REG		0x6A
#define FIFO_COUNTH_REG 	0x72
#define FIFO_R_W_REG 		0x74

#define SAMPLE_NUM			1000
#define RAD_TO_DEG			57.2957795


typedef struct
{
	uint8_t ClockSource;
	uint8_t Gyro_Full_Scale;
	uint8_t Accel_Full_Scale;
	uint8_t CONFIG_DLPF;
	bool 	Sleep_Mode_Bit;

}MPU_ConfigTypeDef;

enum PM_CLKSEL_ENUM
{
	Internal_8MHz 	    = 0x00,
	X_Axis_Ref			= 0x01,
	Y_Axis_Ref			= 0x02,
	Z_Axis_Ref			= 0x03,
	Ext_32_768KHz		= 0x04,
	Ext_19_2MHz			= 0x05,
	TIM_GENT_INREST	    = 0x07
};

enum gyro_FullScale_ENUM
{
	FS_SEL_250 	= 0x00,
	FS_SEL_500 	= 0x01,
	FS_SEL_1000 = 0x02,
	FS_SEL_2000	= 0x03
};

enum accel_FullScale_ENUM
{
	AFS_SEL_2g	= 0x00,
	AFS_SEL_4g,
	AFS_SEL_8g,
	AFS_SEL_16g
};

enum DLPF_CFG_ENUM
{
	DLPF_260A_256G_Hz = 0x00,
	DLPF_184A_188G_Hz = 0x01,
	DLPF_94A_98G_Hz   = 0x02,
	DLPF_44A_42G_Hz   = 0x03,
	DLPF_21A_20G_Hz   = 0x04,
	DLPF_10_Hz 		  = 0x05,
	DLPF_5_Hz 		  = 0x06
};

enum EXT_SYNC_SET_ENUM
{
	input_Disable = 0x00,
	TEMP_OUT_L		= 0x01,
	GYRO_XOUT_L		= 0x02,
	GYRO_YOUT_L		= 0x03,
	GYRO_ZOUT_L		= 0x04,
	ACCEL_XOUT_L	= 0x05,
	ACCEL_YOUT_L	= 0x06,
	ACCEL_ZOUT_L	= 0x07
};

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}RawData_Def;

typedef struct
{
	float x;
	float y;
	float z;
}MeaningData_Def;

typedef struct
{
	float x;
	float y;
	float z;
}CalData_Def;

typedef struct
{
	float yaw;
	float pitch;
	float roll;
}IMU;

IMU accel;
IMU mpu6050;
RawData_Def myAccelRaw, myGyroRaw;
CalData_Def myAccelCal, myGyroCal;
MeaningData_Def myAccelMean, myGyroMean;
float Gyro_Offset_cal_X,Gyro_Offset_cal_Y,Gyro_Offset_cal_Z;
float Accel_Offset_Cal_X,Accel_Offset_Cal_Y,Accel_Offset_Cal_Z;

void MPU6050_Init(I2C_HandleTypeDef *I2Chnd);
void MPU6050_Config(MPU_ConfigTypeDef *config);
uint8_t MPU6050_Get_SMPRT_DIV(void);
void MPU6050_Set_SMPRT_DIV(uint8_t SMPRTvalue);
uint8_t MPU6050_Get_FSYNC(void);
void MPU6050_Set_FSYNC(enum EXT_SYNC_SET_ENUM ext_Sync);
void MPU6050_Get_Accel_RawData(RawData_Def *rawDef);
void MPU6050_Get_Gyro_RawData(RawData_Def *rawDef);
void MPU6050_Get_Cal();
void MPU6050_Read_Value();

#endif /* INC_MPU6050_H_ */
