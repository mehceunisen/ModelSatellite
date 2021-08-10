/*
 * Satellite_Data.h
 *
 *  Created on: 17 May 2021
 *      Author: merty
 */

#ifndef SATELLITE_DATA_H_
#define SATELLITE_DATA_H_
#define MAX_BUF_SIZE 13

enum
{
	ESP_HEADER_1 = 0x1E,
	ESP_HEADER_2 = 0x1F,
};

enum
{
	SEPARATION_MSG_ID = 0x81,
	FOG_BUZZER = 0x82,
	PID_MSG_ID = 0x87,
};

enum
{
	ESP_HEADER_1_YAKALA,
	ESP_HEADER_2_YAKALA,
	MSG_LEN_YAKALA,
	MSG_ID_YAKALA,
	TOPLA_BUF,
	CRC_L_YAKALA,
	CRC_H_YAKALA,
};

typedef struct
{
	float kP;
	float kI;
	float kD;
}Rx_PID;

typedef struct
{
	uint16_t h1;
	uint16_t h2;
	uint16_t empty_pack;
	uint16_t CrC;
}Rx_error;

typedef struct
{
	uint16_t index;
	uint8_t  state;
	uint8_t  pack_data[MAX_BUF_SIZE];
	uint8_t  header1;
	uint8_t  header2;
	uint8_t  len;
	uint8_t  massege_ID;
	Rx_PID 	 pid;
	uint8_t  CrC_L;
	uint8_t  CrC_H;
	Rx_error rx_error;
}ESP32toSTM32;

typedef union
{
	struct
	{
		uint16_t is_separation      : 1;
		uint16_t is_rescue          : 1;
		uint16_t is_rise            : 1;
		uint16_t is_landing         : 1;
		uint16_t is_wait            : 1;
		uint16_t reserved_1         : 3;
		uint16_t separation_command : 1;
		uint16_t is_stabil          : 1;
		uint16_t reserved_2         : 6;
	}bit_status;
	uint16_t all;
}stm32_sat_status;

typedef union
{
	struct
	{
		uint16_t file_transfer      : 1;
		uint16_t sd_log             : 1;
		uint16_t is_time_match      : 1;
		uint16_t reserved_1         : 5;
		uint16_t full_reset         : 1;
		uint16_t time_match         : 1;
		uint16_t video_streaming    : 1;
		uint16_t reset_counter		: 1;
		uint16_t reserved_2         : 4;
	}bit_status;
	uint16_t all;
}esp32_sat_status;

typedef struct{
		 uint8_t   		   header_1;
		 uint8_t           header_2;
		 uint8_t   		   lenght;
		 uint16_t  		   team_num;
		 uint16_t  	 	   pack_num;
		 uint32_t          time;
         uint16_t          pressure;
         uint16_t          altitude;
         uint16_t          velocity;
         uint16_t          temperature;
         uint16_t          voltage;
         uint32_t          gps_latitude;
         uint32_t          gps_longitude;
         uint32_t          gps_altitude;
         stm32_sat_status  stm_satellite_status;
         esp32_sat_status  esp_satellite_status;
         uint16_t  		   pitch;
         uint16_t          roll;
         uint16_t          yaw;
         uint16_t  		   num_turn;
         uint16_t  		   power;
         uint8_t		   rssi;
		 uint8_t	  	   last_command;
		 uint8_t           last_C_state;
         uint16_t          CrC;
		}Telemetry;

uint16_t aeskCRCCalculator(uint8_t *frame, size_t framesize);
void CrateTelemetryPack(Telemetry *ptr_telemetry) ;
void PackageTelemetryPack(Telemetry *telemetry);
void ESP32_Data_Control(ESP32toSTM32 *esp32_data, const uint8_t *tempBuf,uint16_t read_Byte);
void ESP32_Data_Parse(ESP32toSTM32 *esp32_data);
void PID_Data_Parse(ESP32toSTM32 *esp32_data);
void SEP_Data_Parse(ESP32toSTM32 *esp32_data);

#endif /* SATELLITE_DATA_H_ */
