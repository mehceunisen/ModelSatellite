/*
 * Satellite_Data.c
 *
 *  Created on: 17 May 2021
 *      Author: merty
 */

#include "AESK_Data_Pack_lib.h"
#include "Satellite_Data.h"

uint16_t aeskCRCCalculator(uint8_t *frame, size_t framesize)
{
	uint16_t crc16_data = 0;
	uint8_t data = 0;
	for (uint8_t mlen = 0; mlen < framesize; mlen++)
	{
		data = frame[mlen] ^ ((uint8_t) (crc16_data) & (uint8_t) (0xFF));
		data ^= data << 4;
		crc16_data = ((((uint16_t) data << 8) | ((crc16_data & 0xFF00) >> 8)) ^ (uint8_t) (data >> 4) ^ ((uint16_t) data << 3));
	}
	return (crc16_data);
}

void CrateTelemetryPack(Telemetry *telemetry){
	  telemetry->header_1      = HEADER_1;
	  telemetry->header_2      = HEADER_2;
	  telemetry->lenght        = TEL_DATA_LEN;
	  telemetry->pressure      = (uint16_t) (LPS22.Barometer_Pressure * pow(10,EXPONENTIAL_1));
	  telemetry->altitude      = (uint16_t) (LPS22.Barometer_Altitude * pow(10,EXPONENTIAL_1));
	  telemetry->velocity      = (uint16_t) (LPS22.Barometer_Speed * pow(10,EXPONENTIAL_1));
	  telemetry->temperature     = (uint16_t) (36 * pow(10,EXPONENTIAL_1));
	  telemetry->voltage       = (uint16_t) (11 * pow(10,EXPONENTIAL_1));
	  telemetry->gps_latitude  = (uint32_t) (gpsData.latitude_f32 * pow(10,EXPONENTIAL_6));
	  telemetry->gps_longitude = (uint32_t) (gpsData.latitude_f32 * pow(10,EXPONENTIAL_6));
	  telemetry->gps_altitude  = (uint32_t) (gpsData.latitude_f32 * pow(10,EXPONENTIAL_6));
	  telemetry->yaw		   = (uint16_t) ((mpu6050.yaw + 90) * pow(10,EXPONENTIAL_1));
	  telemetry->pitch		   = (uint16_t) ((mpu6050.pitch + 90) * pow(10,EXPONENTIAL_1));
	  telemetry->roll		   = (uint16_t) ((mpu6050.roll + 90) * pow(10,EXPONENTIAL_1));
	  telemetry->num_turn	   = (uint16_t) (10 * pow(10,EXPONENTIAL_1));
	  telemetry->power		   = (uint16_t) (125 * pow(10,EXPONENTIAL_1));
	  telemetry->rssi	  	   = 0;
	  telemetry->last_command   = 0;
	  telemetry->last_C_state   = 0;
	  PackageTelemetryPack(telemetry);
}

void PackageTelemetryPack(Telemetry *telemetry){
	uint16_t index = 0;
	AESK_UINT8toUINT8_LE(&telemetry->header_1, Telemetry_Package, &index);
	AESK_UINT8toUINT8_LE(&telemetry->header_2, Telemetry_Package,	&index);
	AESK_UINT8toUINT8_LE(&telemetry->lenght, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->team_num, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->pack_num, Telemetry_Package, &index);
	AESK_UINT32toUINT8_LE(&telemetry->time, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->pressure, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->altitude, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->velocity, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->temperature, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->voltage, Telemetry_Package,&index);
	AESK_UINT32toUINT8_LE(&telemetry->gps_latitude, Telemetry_Package, &index);
	AESK_UINT32toUINT8_LE(&telemetry->gps_longitude, Telemetry_Package, &index);
	AESK_UINT32toUINT8_LE(&telemetry->gps_altitude, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->stm_satellite_status.all, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->esp_satellite_status.all, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->pitch, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->roll, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->yaw, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->num_turn, Telemetry_Package, &index);
	AESK_UINT16toUINT8_LE(&telemetry->power, Telemetry_Package,&index);
	AESK_UINT8toUINT8_LE(&telemetry->rssi, Telemetry_Package, &index);
	AESK_UINT8toUINT8_LE(&telemetry->last_command, Telemetry_Package, &index);
	AESK_UINT8toUINT8_LE(&telemetry->last_C_state, Telemetry_Package, &index);
	telemetry->CrC = aeskCRCCalculator((Telemetry_Package + 3), TEL_DATA_LEN);
	AESK_UINT16toUINT8_LE(&telemetry->CrC, Telemetry_Package,&index);
}

void ESP32_Data_Control(ESP32toSTM32 *esp32_data, const uint8_t *tempBuf, uint16_t read_Byte)
{
	esp32_data->index = 1;
	for(uint16_t j = 0; j < read_Byte; j++)
	{
		switch(esp32_data->state)
		{
			case ESP_HEADER_1_YAKALA :
			{
				if(tempBuf[j] == ESP_HEADER_1)
				{
					esp32_data->header1 = tempBuf[j];
					esp32_data->state = ESP_HEADER_2_YAKALA;
				}
				else
				{
					esp32_data->rx_error.h1++;
				}
				break;
			}

			case ESP_HEADER_2_YAKALA :
			{
				if(tempBuf[j] == ESP_HEADER_2)
				{
					esp32_data->header2 = tempBuf[j];
					esp32_data->state = MSG_LEN_YAKALA;
				}
				else
				{
					esp32_data->rx_error.h2++;
					esp32_data->state = ESP_HEADER_1_YAKALA;
				}
				break;
			}

			case MSG_LEN_YAKALA :
			{
				if(tempBuf[j] != 0)
				{
					esp32_data->len = tempBuf[j];
					esp32_data->state = MSG_ID_YAKALA;
				}
				else
				{
					esp32_data->rx_error.empty_pack++;
					esp32_data->state = ESP_HEADER_1_YAKALA;
				}
				break;
			}

			case MSG_ID_YAKALA :
			{
				esp32_data->massege_ID = tempBuf[j];
				esp32_data->pack_data[0] = esp32_data->massege_ID;
				esp32_data->state = TOPLA_BUF;
				break;
			}

			case TOPLA_BUF :
			{
				if(esp32_data->len != esp32_data->index)
				{
					esp32_data->pack_data[esp32_data->index++] = tempBuf[j];
				}
				else
				{
					esp32_data->index = 1;
					esp32_data->state = CRC_L_YAKALA;
				}
				break;
			}

			case CRC_L_YAKALA :
			{
				esp32_data->CrC_L = tempBuf[j-1];
				esp32_data->state = CRC_H_YAKALA;
				j -= 1;
				break;
			}

			case CRC_H_YAKALA :
			{
				esp32_data->CrC_H = tempBuf[j];
				uint16_t calc_CrC = aeskCRCCalculator(esp32_data->pack_data, esp32_data->len);
				uint16_t real_CrC = ((esp32_data->CrC_L) | (esp32_data->CrC_H << 8));
				if(real_CrC == calc_CrC)
				{
					ESP32_Data_Parse(esp32_data);
				}
				else
				{
					esp32_data->rx_error.CrC++;
				}
				esp32_data->index = 1;
				esp32_data->state = ESP_HEADER_1_YAKALA;
				break;
			}
		}
	}
}

void ESP32_Data_Parse(ESP32toSTM32 *esp32_data)
{
	uint16_t indeks  = 1;
	switch(esp32_data->massege_ID)
	{
		case SEPARATION_MSG_ID :
		{

			break;
		}

		case FOG_BUZZER :
		{

			break;
		}

		case PID_MSG_ID :
		{
			AESK_UINT8toFLOAT32_LE(&esp32_data->pid.kP,esp32_data->pack_data, &indeks);
			AESK_UINT8toFLOAT32_LE(&esp32_data->pid.kI,esp32_data->pack_data, &indeks);
			AESK_UINT8toFLOAT32_LE(&esp32_data->pid.kD,esp32_data->pack_data, &indeks);
			break;
		}


	}
}
