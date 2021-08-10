/*
 * gy271.c
 *
 *  Created on: Jun 9, 2021
 *      Author: egemen
 */

#include "gy271.h"
#include "math.h"

float mag_off_max_x=0,mag_off_max_y=0,mag_off_max_z=0,mag_off_min_x=0,mag_off_min_y=0,mag_off_min_z=0;

void ManyometerWrite(uint8_t wRegister,uint8_t Value)
{
	uint8_t data[2];
	data[0]         = wRegister;
	data[1]         = Value;
	HAL_I2C_Master_Transmit(&hi2c3, ManyometerSlaveAdressWrite, data, 2, 10);

}
void ManyometerRead(uint8_t rRegister,uint8_t NumberOfBytes)
{
	HAL_I2C_Mem_Read(&hi2c3, ManyometerSlaveAdressRead, rRegister, 1,Mydatas, NumberOfBytes, 10);
}
void ManyometerInit()
{
    // baslangıc kurulumu
	HAL_Delay(10);

	ManyometerWrite(HMC5883L_MODE_REGISTER,0x00);
	ManyometerWrite(HMC5883L_CFG_REG_A,0b00010000);
	ManyometerWrite(HMC5883L_CFG_REG_B,0b00100000);

	calibration_matrice[0][0]=0.141937;
	calibration_matrice[0][1]=0.000479;
	calibration_matrice[0][2]=0.000078;
    calibration_matrice[1][0]=0.000479;
	calibration_matrice[1][1]=0.116996;
	calibration_matrice[1][2]=0.000102;
    calibration_matrice[2][0]=0.000078;
	calibration_matrice[2][1]=0.000102;
	calibration_matrice[2][2]=0.183908;

    bias_value_mag_x=-11.875537;
    bias_value_mag_y=-621.8516;
    bias_value_mag_z=-64.713503;

    magno_declination=5.50;
}
void ManyometerReadValue()
{
	ManyometerRead(0x03, 6);
	Mag_Raw_X=(Mydatas[0] << 8) | Mydatas[1];
	Mag_Raw_Z=(Mydatas[2] << 8) | Mydatas[3];
	Mag_Raw_Y=(Mydatas[4] << 8) | Mydatas[5];

	scaled_mag_x=Mag_Raw_X * 0.92; //1,3 mg
    scaled_mag_y=Mag_Raw_Y * 0.92;
	scaled_mag_z=Mag_Raw_Z * 0.92;

    biased_mag_x = scaled_mag_x - bias_value_mag_x;
    biased_mag_y = scaled_mag_y - bias_value_mag_y;
    biased_mag_z = scaled_mag_z - bias_value_mag_z;

    calibrated_mag_x = calibration_matrice[0][0]*biased_mag_x + calibration_matrice[0][1]*biased_mag_y + calibration_matrice[0][2]*biased_mag_z;
    calibrated_mag_y = calibration_matrice[1][0]*biased_mag_x + calibration_matrice[1][1]*biased_mag_y + calibration_matrice[1][2]*biased_mag_z;
    calibrated_mag_z = calibration_matrice[2][0]*biased_mag_x + calibration_matrice[2][1]*biased_mag_y + calibration_matrice[2][2]*biased_mag_z;

}
void Magno_Offset_Calc()
{
	static int i;

	for(i=0;i<100000;i++) //while tarzı
	{
		if(Mag_Raw_X>mag_off_max_x)
		{
			mag_off_max_x = Mag_Raw_X;
		}
		if(Mag_Raw_X<mag_off_min_x)
		{
			mag_off_min_x = Mag_Raw_X;
		}
		if(Mag_Raw_Y>mag_off_max_y)
		{
			mag_off_max_y = Mag_Raw_Y;
		}
		if(Mag_Raw_Y<mag_off_min_y)
		{
			mag_off_min_y = Mag_Raw_Y;
		}
		if(Mag_Raw_Z>mag_off_max_z)
		{
			mag_off_max_z = Mag_Raw_Z;
		}
		if(Mag_Raw_Z<mag_off_min_z)
		{
			mag_off_min_z = Mag_Raw_Z;
		}
	}

}
