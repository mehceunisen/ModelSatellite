
#ifndef MAIN_AESK_DATA_FLOW_H_
#define MAIN_AESK_DATA_FLOW_H_
#include <stdint.h>


#include "esp_system.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>

#define FLOAT_CONVERTER_1	10
#define FLOAT_CONVERTER_2 	100
#define FLOAT_CONVERTER_3 	1000
#define FLOAT_CONVERTER_6 	1000000


#define HEADER_1			0x13
#define HEADER_2 			0x17

#define ESP_COMMAND_H0 10
#define ESP_COMMAND_H1 71

#define STM_COMMAND_H0 30
#define STM_COMMAND_H1 31

#define ESP_RESET 1
#define FULL_RESET 2
#define SET_MATRIX 11
#define SYNC_TIME 19
#define SET_TEAM_NUMBER 21
#define ESP_MATRIX_ID 0
#define STM_MATRIX_ID 1
#define SET_TASK 30

//Esp Matrix
typedef union
{
	struct
	{
	uint8_t x0_is_doing_ftp       : 1;
	uint8_t x1_has_done_ftp       : 1;
	uint8_t x2_is_time_synch      : 1;
	uint8_t x3_  : 1;

    uint8_t x4_  : 1;
	uint8_t x5_  : 1;
	uint8_t x6_  : 1;
	uint8_t x7_  : 1;

    uint8_t x8_ar_full_reset      : 1;
	uint8_t x9_ar_synch_time      : 1;
	uint8_t xa_ar_do_ftp          : 1;
	uint8_t xb_ar_reset_counters  : 1;

    uint8_t xc_ar_				  : 1;
	uint8_t xd_ar_				  : 1;
	uint8_t xe_ar_no_video_stream : 1;
	uint8_t xf_ar_no_sd_mode      : 1;

	}Flags;
	uint16_t  AsNumber;
}ESP_Matrix_Union;

//STM Matrix
typedef union
{
	struct
	{
	uint8_t x_0  : 1;
	uint8_t x_1  : 1;
	uint8_t x_2  : 1;
	uint8_t x_3  : 1;

    uint8_t x_4  : 1;
	uint8_t x_5  : 1;
	uint8_t x_6  : 1;
	uint8_t x_7  : 1;

    uint8_t x_8  : 1;
	uint8_t x_9  : 1;
	uint8_t x_a  : 1;
	uint8_t x_b  : 1;

    uint8_t x_c  : 1;
	uint8_t x_d  : 1;
	uint8_t x_e  : 1;
	uint8_t x_f  : 1;

	}Flags;
	uint16_t  AsNumber;
}STM_Matrix_Union;

typedef struct
{
    /* data */
    uint16_t team_number;			//! ESP VAR Her takıma verilen takım numarası
    uint16_t packet_number;			//! ESP NVS Gönderilen paketlerin numarası
    uint32_t transmit_time;			//! ESP NTP,NVS Gerçek Zaman
    uint16_t press;					//? STM UART SEN Basınç verisi
    uint16_t altitute_m;			//? STM UART SEN Yükseklik
    uint16_t velocity_mds;			//? STM UART SEN Hız
    uint16_t temperature;			//? STM UART SEN Sıcaklık
    uint16_t battery_voltage;		//? STM UART SEN Batarya gerilimi
    uint32_t gps_lat;				//? STM UART SEN GPS lat
    uint32_t gps_long;				//? STM UART SEN GPS long
    uint32_t gps_alt;				//? STM UART SEN GPS alt
    STM_Matrix_Union stm_status;	//? STM UART Stm Durum Matrisi
    ESP_Matrix_Union esp_status;	//! ESP NVS ESp Durum Matrisi
    uint16_t pitch;					//? STM UART SEN IMU pitch
    uint16_t roll;					//? STM UART SEN IMU roll
    uint16_t yaw;					//? STM UART SEN IMU yaw
    uint16_t roll_counter;			//? STM UART VAR dönüş sayısı
									//* Ek veriler
    uint16_t motor_power;			//? STM UART VAR P = I*V

	int8_t rssi;					//! ESP VAR Wifi Aktarılan Güç -dbm
	uint8_t last_command_code;		//! ESP VAR En son çalıştırılmış komut
	uint8_t last_command_state; 	//! ESP VAR EN son çalıştırılmış komut başarılı mı?
	/*
	float kp_roll_pitch;			//? STM UART VAR PID Kp roll & pitch
	float ki_roll_pitch;			//? STM UART VAR PID Ki roll & pitch
	float kd_roll_pitch;			//? STM UART VAR PID Kd roll & pitch

	float error_roll 				//? STM UART VAR PID error roll
	float integral_roll 			//? STM UART VAR PID integral roll
	float pwm_signal_duty_roll  	//? STM UART VAR PID pwm roll

	float error_pitch 				//? STM UART VAR PID error pitch
	float integral_pitch 			//? STM UART VAR PID integral pitch
	float pwm_signal_duty_pitch  	//? STM UART VAR PID pwm pitch

	float kp_thrust;				//? STM UART VAR PID Kp thrust
	float ki_thrust;				//? STM UART VAR PID Ki thrust
	float kd_thrust;				//? STM UART VAR PID Kd thrust
	float pwm_signal_duty_thrust  	//? STM UART VAR PID pwm thrust
	*/
}Telemetry_Packet;


//Packet Decompose State Enum
typedef enum{
	catch_header_1,
	catch_header_2,
	get_len,
	add_buffer,
	crc_l_check,
	crc_h_check
}Decompose_States;

uint16_t getu16_from2u8(uint8_t l_byte,uint8_t m_byte);

uint16_t aeskCRCCalculator(uint8_t *frame, size_t framesize);

esp_err_t log_string_g(char* logString, Telemetry_Packet data);

void test_init_variable(Telemetry_Packet* data);

/* PACKAGE FUNCTIONS */
void AESK_UINT16toUINT8_LE(uint16_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT16toUINT8_LE(int16_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT32toUINT8_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT32toUINT8_LE(int32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_FLOAT32toUINT8_LE(float *packData, uint8_t *packBuf, uint16_t *index);
void AESK_FLOAT64toUINT8_LE(double *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toUINT8CODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT8toUINT8CODE(int8_t *packData, uint8_t *packBuf, uint16_t *index);
/*ENCODE FUNCTIONS */
void AESK_UINT8toINT16_LE(int16_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toUINT16_LE(uint16_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toINT32_LE(int32_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toUINT32_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toUINT8ENCODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toINT8ENCODE(int8_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toFLOAT32_LE(float *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toFLOAT64_LE(double *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);
void AESK_INT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);


#endif /* MAIN_AESK_DATA_FLOW_H_ */
