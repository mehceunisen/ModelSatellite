#include "main.h"
#include "math.h"
#include "lps22hd.h"


void BarometerWrite(uint8_t wRegister,uint8_t Value)
{
	uint8_t data[2];
	data[0]         = wRegister;
	data[1]         = Value;
	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR_WRITE, data, 2, 10);

}
void BarometerReadXL(uint8_t rRegister,uint8_t NumberOfBytes)
{
	HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDR_READ, rRegister, 1 , Barometer_DataXL, NumberOfBytes, 10);

}
void BarometerReadL(uint8_t rRegister,uint8_t NumberOfBytes)
{
	HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDR_READ, rRegister, 1 , Barometer_DataL, NumberOfBytes, 10);

}
void BarometerReadH(uint8_t rRegister,uint8_t NumberOfBytes)
{
	HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDR_READ, rRegister, 1 , Barometer_DataH, NumberOfBytes, 10);

}
void BarometerInit()
{

	BarometerWrite(CTRL_REG1,0b01010000);
	BarometerWrite(CTRL_REG2,0x00);
}
void BarometerReadValues()
{
	  BarometerReadXL(PRESS_OUT_XL, 1);
	  BarometerReadL(PRESS_OUT_L, 1);
	  BarometerReadH(PRESS_OUT_H, 1);

	  LPS22.Barometer_Pressure = Barometer_DataXL[0]  + (Barometer_DataL[0]<<8) + (Barometer_DataH[0]<<16);
	  LPS22.Barometer_Pressure = LPS22.Barometer_Pressure/4096;

}

void BarometerAltitude()
{
	LPS22.Barometer_Altitude = 44300*(1.0-pow((double)LPS22.Barometer_Pressure / LPS22.First_Barometer_Pressure, 0.1903));
}

//void ZeroPBarometer(){
//	for(uint16_t i=0;i<1000;i++){
//		BarometerReadValues();
//		LPS22.First_Barometer_Pressure += LPS22.Barometer_Pressure;
//		HAL_Delay(5);
//	}
//	LPS22.First_Barometer_Pressure /= 1000;
//}
