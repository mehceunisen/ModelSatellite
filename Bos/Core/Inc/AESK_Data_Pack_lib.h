/*
 * AESK_Data_Pack_lib.h
 *
 *  Created on: 17 Eki 2019
 *      Author: yemrelaydin
 */

#ifndef AESK_DATA_PACK_LIB_H_
#define AESK_DATA_PACK_LIB_H_
#include "stdint.h"
#include "main.h"
#include "GPS.h"
#include "MPU6050.h"
#include "lps22hd.h"

#define HEADER_1                    19
#define HEADER_2                    23
#define TEL_DATA_LEN				47
#define EXPONENTIAL_1 			    1
#define EXPONENTIAL_6 			    6

uint8_t Telemetry_Package[52];
/*
 * BAŞLANGIÇ  (31)  +               -> 1 byte [0]
 * GÖNDERİM TARİHİ	+				-> 8 byte [1]-[8]
 * BASINÇ			+				-> 2 byte [9][11]
 * YÜKSEKLİK		+	            -> 2 byte [12][13]
 * İNİŞ HIZI        +  				-> 2 byte [14][15]
 * SICAKLIK		 	+				-> 2 byte [16][17]
 * PİL GERİLİMİ		+				-> 2 byte [18][19]
 * GPS LATITUDE		+				-> 4 byte [20]-[23]
 * GPS LONGITUDE	+				-> 4 byte [24]-[27]
 * GPS ALTITUDE		+				-> 4 byte [28]-[31]
 * UYDU STATÜSÜ		+				-> 2 byte [32][33]
 * YAW				+				-> 2 byte [34][35]
 * PİTCH			+				-> 2 byte [36][37]
 * ROLL				+				-> 2 byte [38][39]
 * DÖNÜŞ SAYISI		+				-> 2 byte [40][41]
 * ANLIK GÜÇ		+				-> 2 byte [42][43]
 * SON  (69)  		+               -> 1 byte [44]
 */


/* PACKAGE FUNCTIONS */
void AESK_UINT16toUINT8_LE(uint16_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT16toUINT8_BE(uint16_t *packData, uint8_t *packBuf, uint16_t *index);

void AESK_INT16toUINT8_LE(int16_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT16toUINT8_BE(int16_t *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT32toUINT8_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT32toUINT8_BE(uint32_t *packData, uint8_t* packBuf, uint16_t *index);

void AESK_INT32toUINT8_LE(int32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT32toUINT8_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index);

void AESK_FLOAT32toUINT8_LE(float *packData, uint8_t *packBuf, uint16_t *index);
void AESK_FLOAT32toUINT8_BE(float *packData, uint8_t *packBuf, uint16_t *index);

void AESK_FLOAT64toUINT8_LE(double *packData, uint8_t *packBuf, uint16_t *index);
void AESK_FLOAT64toUINT8_BE(double *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT8toUINT8CODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_INT8toUINT8CODE(int8_t *packData, uint8_t *packBuf, uint16_t *index);


/*ENCODE FUNCTIONS */
void AESK_UINT8toINT16_LE(int16_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toINT16_BE(int16_t *packData, uint8_t* packBuf, uint16_t *index);

void AESK_UINT8toUINT16_LE(uint16_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toUINT16_BE(uint16_t *packData, uint8_t* packBuf, uint16_t *index);

void AESK_UINT8toINT32_LE(int32_t *packData, uint8_t* packBuf, uint16_t *index);
void AESK_UINT8toINT32_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index);

void AESK_UINT8toUINT32_BE(uint32_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toUINT32_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT8toUINT8ENCODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toINT8ENCODE(int8_t *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT8toFLOAT32_LE(float *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toFLOAT32_BE(float *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT8toFLOAT64_LE(double *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toFLOAT64_BE(double *packData, uint8_t *packBuf, uint16_t *index);

void AESK_UINT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);
void AESK_UINT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);

void AESK_INT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);
void AESK_INT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index);

void AESK_UINT64toUINT8_LE(uint64_t *packData, uint8_t *packBuf, uint16_t *index);
void AESK_UINT8toUINT8_LE(uint8_t *packData, uint8_t *packBuf, uint16_t *index);



#endif /* AESK_DATA_PACK_LIB_H_ */
