#include "aesk_data_flow.h"


uint16_t getu16_from2u8(uint8_t l_byte,uint8_t m_byte){
    uint16_t target_u16 = 0;
    target_u16 |= m_byte<<8;
    target_u16 |=l_byte;
    return target_u16;
}

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


esp_err_t log_string_g(char* logString, Telemetry_Packet data){

	char timebuffer[30];
	struct tm* timeinfo;
	timeinfo = localtime((time_t*)(&data.transmit_time));
	strftime(timebuffer, 30, "%d/%m/%Y, %H:%M:%S", timeinfo);
	//adamların istediği formatta yaz işte
	//sprintf()
	sprintf(logString,"%d,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.6f,%.6f,%.6f,%x%x,%.2f,%.2f,%.2f,%d,%.2f,%d,%x,%x"
		,data.team_number,
		data.packet_number,
		timebuffer,
		(float)data.press/FLOAT_CONVERTER_1,
		(float)data.altitute_m/FLOAT_CONVERTER_1,
		(float)data.velocity_mds/FLOAT_CONVERTER_1,
		(float)data.temperature/FLOAT_CONVERTER_1,
		(float)data.battery_voltage/FLOAT_CONVERTER_1,
		(float)data.gps_lat/FLOAT_CONVERTER_6,
		(float)data.gps_long/FLOAT_CONVERTER_6,
		(float)data.gps_alt/FLOAT_CONVERTER_6,
		data.stm_status.AsNumber,
		data.esp_status.AsNumber,
		((float)data.pitch/FLOAT_CONVERTER_1-90),
		((float)data.roll/FLOAT_CONVERTER_1-90),
		((float)data.yaw/FLOAT_CONVERTER_1-90),
		data.roll_counter,
		(float)data.motor_power/FLOAT_CONVERTER_1,
		data.rssi,
		data.last_command_code,
		data.last_command_state
		);

    return ESP_OK;
}


void test_init_variable(Telemetry_Packet* data){
	data->packet_number=0;
	data->altitute_m=0;
	data->battery_voltage=0;
	data->esp_status.AsNumber=0;
	data->gps_alt=0;
	data->gps_lat=0;
	data->gps_long=0;
	data->last_command_code=0;
	data->last_command_state=0;
	data->motor_power=0;

	data->pitch=0;
	data->press=0;
	data->roll =0;
	data->roll_counter =0;
	data->rssi = 0;
	data->stm_status.AsNumber =0;
	data->team_number =37833;
	data->temperature = 0;
	data->transmit_time=0;
	data->velocity_mds=0;
	data->yaw=0;
}

/***************************************  DATA PACKAGE START   ********************************************************/
/*
 *  BU KÜTÜPHANE LITTLE ENDIAN ve IEEE 754 FLOATING POINT BULUNAN İŞLEMCİLER İÇİN TASARLANMIŞTIR.
 *  EĞER İŞLEMCİNİZ BIG ENDIAN YAPISINDAYSA _LE ve _BE BİRBİRİNİN TERSİ OLARAK ÇALIŞACAKTIR.
 *
 */

/***************************************  UINT16 PACKAGE START ********************************************************/
/* Bu fonksiyon uint16_t türünden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasarımda  düşük anlamlı bayt düşük indekse aktarılır.
 * Parametreler : uint16_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanacağı dizi girilir.
 * 				: uint16_t *index    = paketin indeks sırası girilir.
 * �RNEK:
 *  		uint16_t yemre = 0x4555;
 *  		uint8_t dizi[2];
 *
 *  		dizi[0] = 0x55;
 *  		dizi[1] = 0x45;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT16toUINT8_LE(uint16_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];

	*index = *index + sizeof(uint16_t);
}


/***************************************  INT16 PACKAGE END *********************************************************/

/***************************************  UINT16 PACKAGE START ********************************************************/
/* Bu fonksiyon int16_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt d���k indekse aktar�l�r.
 * Parametreler : uint16_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		int16_t yemre = 0x4555;
 *  		uint8_t dizi[2];
 *
 *  		dizi[0] = 0x55;
 *  		dizi[1] = 0x45;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_INT16toUINT8_LE(int16_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];

	*index = *index + sizeof(int16_t);
}


/***************************************  INT16 PACKAGE END *********************************************************/


/***************************************  UINT32 PACKAGE START *********************************************************/

/* Bu fonksiyon uint32_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt d���k indekse aktar�l�r.
 * Parametreler : uint32_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint32_t yemre = 0x45550012;
 *  		uint8_t dizi[4];
 *
 *  		dizi[0] = 0x12;
 *  		dizi[1] = 0x00;
 *  		dizi[2] = 0x55;
 *  		dizi[3] = 0x45;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT32toUINT8_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(uint32_t);
}

/***************************************  UINT32 PACKAGE END *********************************************************/

/***************************************  UINT32 PACKAGE START *********************************************************/

/* Bu fonksiyon int32_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt d���k indekse aktar�l�r.
 * Parametreler : uint32_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint32_t yemre = 0x45550012;
 *  		uint8_t dizi[4];
 *
 *  		dizi[0] = 0x12;
 *  		dizi[1] = 0x00;
 *  		dizi[2] = 0x55;
 *  		dizi[3] = 0x45;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_INT32toUINT8_LE(int32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(int32_t);
}


/***************************************  INT32 PACKAGE END *********************************************************/

/***************************************  FLOAT32 PACKAGE START *********************************************************/

void AESK_FLOAT32toUINT8_LE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(float);
}

/***************************************  FLOAT32 PACKAGE END *********************************************************/
/***************************************  FLOAT64(DOUBLE) PACKAGE START *********************************************************/

void AESK_FLOAT64toUINT8_LE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 4] = ((uint8_t*)packData)[4];
	packBuf[(*index) + 5] = ((uint8_t*)packData)[5];
	packBuf[(*index) + 6] = ((uint8_t*)packData)[6];
	packBuf[(*index) + 7] = ((uint8_t*)packData)[7];

	*index = *index + sizeof(double);
}

/***************************************  FLOAT64(DOUBLE) PACKAGE END *********************************************************/
/***************************************  UINT8 PACKAGE START *********************************************************/

void AESK_UINT8toUINT8CODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];

	*index = *index + sizeof(uint8_t);
}

void AESK_INT8toUINT8CODE(int8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(int8_t);
}
/***************************************  UINT8 PACKAGE END *********************************************************/
/***************************************  DATA PACKAGE END  *********************************************************/



/***************************************  DATA ENCODE START  *********************************************************/

/***************************************  INT16_T ENCODE START  *********************************************************/
/* Bu fonksiyon uint8_t t�r�nden veriyi int16_t olarak paketlemeye yarar.
 * Buradaki tasar�m Little Endian paketlenmi� veriyi birle�tirir.
 * Parametreler : int16_t *packData = paketlin a��laca�� verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketlenen dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		int16_t yemre;
 *  		uint8_t dizi[4];
 *  		dizi[0] = 0x00;
 *  		dizi[1] = 0x12
 *  		yemre = 0x1200;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT8toINT16_LE(int16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[0] = packBuf[*index];
	((uint8_t *)packData)[1] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

/***************************************  INT16_T ENCODE END  *********************************************************/




/***************************************  UINT16_T ENCODE START  *********************************************************/

/* Bu fonksiyon uint8_t t�r�nden veriyi int16_t olarak paketlemeye yarar.
 * Buradaki tasar�m Little Endian paketlenmi� veriyi birle�tirir.
 * Parametreler : uint32_t *packData = paketlin a��laca�� verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketlenen dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint16_t yemre;
 *  		uint8_t dizi[4];
 *  		dizi[2] = 0x00;
 *  		dizi[3] = 0x12
 *  		yemre = 0x1200;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT8toUINT16_LE(uint16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[0] = packBuf[*index];
	((uint8_t *)packData)[1] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

/***************************************  UINT16_T ENCODE END  *********************************************************/


/***************************************  INT32_T ENCODE START  *********************************************************/
void AESK_UINT8toINT32_LE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *) packData)[0] = packBuf[*index];
	((uint8_t *) packData)[1] = packBuf[(*index) + 1];
	((uint8_t *) packData)[2] = packBuf[(*index) + 2];
	((uint8_t *) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(int32_t);
}
/***************************************  INT32_T ENCODE END  *********************************************************/

/***************************************  UINT32_T ENCODE START  *********************************************************/

void AESK_UINT8toUINT32_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(uint32_t);
}

/***************************************  UINT32_T ENCODE END  *********************************************************/


/***************************************  UINT8_T - INT8_T ENCODE START  *********************************************************/
void AESK_UINT8toUINT8ENCODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];

	*index = *index + sizeof(uint8_t);
}

void AESK_UINT8toINT8ENCODE(int8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];

	*index = *index + sizeof(uint8_t);
}
/***************************************  UINT8_T - INT8_T ENCODE END  *********************************************************/

/***************************************  FLOAT32 ENCODE START  *********************************************************/
void AESK_UINT8toFLOAT32_LE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(float);
}

/***************************************  FLOAT32 ENCODE END  *********************************************************/

/***************************************  FLOAT64 ENCODE START  *********************************************************/
void AESK_UINT8toFLOAT64_LE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];
	((uint8_t*) packData)[4] = packBuf[(*index) + 4];
	((uint8_t*) packData)[5] = packBuf[(*index) + 5];
	((uint8_t*) packData)[6] = packBuf[(*index) + 6];
	((uint8_t*) packData)[7] = packBuf[(*index) + 7];
	*index = *index + sizeof(double);
}

/***************************************  FLOAT64 ENCODE END  *********************************************************/

/*
 * BU B�L�M AESK VER� YAPISINA �ZEL OLARAK TASARLANMI�TIR. AESK VER� YAPISI MANTI�INDA KULLANILAN
 * FLOAT DE�ER�N B�R KAT SAYI �LE �ARPILARAK INTEGER HALINE GET�R�LMES� KULLANILARAK YAPILAN TASARIMDA
 * FONKS�YON ��ER�S�NDE YER ALAN CONVERTER PARAMETRES�:
 *
 *   FLOAT_CONVERTER_1 			10
 *	 FLOAT_CONVERTER_2 			100
 *	 FLOAT_CONVERTER_3 			1000
 *
 *	 OLARAK KAYITLIDIR.
 *
 */
void AESK_UINT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	uint16_t data;
	((uint8_t *)&data)[0] = packBuf[*index];
	((uint8_t *)&data)[1] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;
	*index = *index + sizeof(uint16_t);
}

/*
 * BU B�L�M AESK VER� YAPISINA �ZEL OLARAK TASARLANMI�TIR. AESK VER� YAPISI MANTI�INDA KULLANILAN
 * FLOAT DE�ER�N B�R KAT SAYI �LE �ARPILARAK INTEGER HALINE GET�R�LMES� KULLANILARAK YAPILAN TASARIMDA
 * FONKS�YON ��ER�S�NDE YER ALAN CONVERTER PARAMETRES�:
 *
 *   FLOAT_CONVERTER_1 			10
 *	 FLOAT_CONVERTER_2 			100
 *	 FLOAT_CONVERTER_3 			1000
 *
 *	 OLARAK KAYITLIDIR.
 *
 */
void AESK_INT16toFLOAT_LE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	int16_t data;
	((uint8_t *)&data)[0] = packBuf[*index];
	((uint8_t *)&data)[1] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;

	*index = *index + sizeof(int16_t);
}




