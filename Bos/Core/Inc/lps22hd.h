#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdbool.h>	
#include <math.h>

I2C_HandleTypeDef hi2c1;

#define SA0STATE 1

#if   (SA0STATE == 0)
	#define 	LSP22HD_ADDR		0x5C
#elif (SA0STATE == 1)
	#define		LSP22HD_ADDR		0x5D
#endif

//defining reigsters 
#define SLAVE_ADDR_WRITE  0xBA
#define SLAVE_ADDR_READ   0xBB

#define INTERRUPT_CFG 0x0B
#define THS_P_L       0x0C
#define THS_P_H       0x0D
#define WHO_AM_I      0x0F
#define CTRL_REG1     0x10
#define CTRL_REG2     0x11
#define CTRL_REG3     0x12
#define FIFO_CTRL     0x14
#define REF_P_XL      0x15 
#define REF_P_L       0x16
#define REF_P_H       0x17
#define RPDS_L        0x18
#define RPDS_H        0x19
#define RES_CONF      0x1A
#define INT_SOURCE    0x25
#define FIFO_STATUS   0x26
#define STATUS        0x27
#define PRESS_OUT_XL  0x28
#define PRESS_OUT_L   0x29
#define	PRESS_OUT_H   0x2A
//#define TEMP_OUT_L    0x2B
#define TEMP_OUT_H    0x2C
#define LPFP_RES      0x33

#define    P_1shot  0x00
#define    P_1Hz    0x01
#define    P_10Hz   0x02
#define    P_25Hz   0x03  // 25 Hz output data rate
#define    P_50Hz   0x04
#define    P_75Hz   0x05



uint8_t Barometer_DataXL[1];
uint8_t Barometer_DataL[1];
uint8_t Barometer_DataH[1];
float Barometer_Pressure;
float First_Barometer_Pressure;
Barometer LPS22;

void BarometerInit();
void BarometerReadXL(uint8_t rRegister,uint8_t NumberOfBytes);
void BarometerReadL(uint8_t rRegister,uint8_t NumberOfBytes);
void BarometerReadH(uint8_t rRegister,uint8_t NumberOfBytes);
void BarometerWrite(uint8_t wRegister,uint8_t Value);
void BarometerReadValues();
void BarometerAltitude();
//void ZeroPBarometer();

