/*
 * AESK_Data_Pack_lib.c
 *
 *  Created on: 17 Eki 2019
 *      Author: yemrelaydin
 */
#include "AESK_Data_Pack_Lib.h"


/***************************************  DATA PACKAGE START   ********************************************************/
/*
 *  BU K�T�PHANE LITTLE ENDIAN ve IEEE 754 FLOATING POINT bulunan ��LEMC�LER ���N TASARLANMI�TIR. 
 *	BU K�T�PHANEY� KULLANMADAN �NCE L�TFEN ��LEMC�N�Z�N HAFIZA Y�NET�M YAPISINI �NCELEY�N�Z.
 *  E�ER ��LEMC�N�Z BIG ENDIAN YAPISINDAYSA _LE ve _BE B�RB�R�N�N TERS� OLARAK �ALI�ACAKTIR.
 *
 *
 */

/***************************************  UINT16 PACKAGE START ********************************************************/
/* Bu fonksiyon uint16_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt d���k indekse aktar�l�r.
 * Parametreler : uint16_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
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



/* Bu fonksiyon uint16_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt y�ksek indekse aktar�l�r.
 * Parametreler : uint16_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint16_t yemre = 0x4555;
 *  		uint8_t dizi[2];
 *
 *  		dizi[0] = 0x45;
 *  		dizi[1] = 0x55;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT16toUINT8_BE(uint16_t *packData, uint8_t *packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[0];

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



/* Bu fonksiyon int16_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt y�ksek indekse aktar�l�r.
 * Parametreler : uint16_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		int16_t yemre = 0x4555;
 *  		uint8_t dizi[2];
 *
 *  		dizi[0] = 0x45;
 *  		dizi[1] = 0x55;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_INT16toUINT8_BE(int16_t *packData, uint8_t *packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[0];

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


/* Bu fonksiyon uint32_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt y�ksek indekse aktar�l�r.
 * Parametreler : uint32_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint32_t yemre = 0x45550012;
 *  		uint8_t dizi[4];
 *
 *  		dizi[0] = 0x45;
 *  		dizi[1] = 0x55;
 *  		dizi[2] = 0x00;
 *  		dizi[3] = 0x12;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT32toUINT8_BE(uint32_t *packData, uint8_t* packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[3];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
		packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

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


/* Bu fonksiyon int32_t t�r�nden veriyi uint8_t olarak paketlemeye yarar.
 * Buradaki tasar�mda d���k anlaml� bayt y�ksek indekse aktar�l�r.
 * Parametreler : uint32_t *packData = paketlenecek verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketin toplanaca�� dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		int32_t yemre = 0x45550012;
 *  		uint8_t dizi[4];
 *
 *  		dizi[0] = 0x45;
 *  		dizi[1] = 0x55;
 *  		dizi[2] = 0x00;
 *  		dizi[3] = 0x12;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_INT32toUINT8_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[3];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
		packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

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

void AESK_FLOAT32toUINT8_BE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

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

void AESK_FLOAT64toUINT8_BE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[7];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[6];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[5];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[4];
	packBuf[(*index) + 4] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 5] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 6] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 7] = ((uint8_t*)packData)[0];

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




/* Bu fonksiyon uint8_t t�r�nden veriyi int16_t olarak paketlemeye yarar.
 * Buradaki tasar�m Big Endian paketlenmi� veriyi birle�tirir.
 * Parametreler : int16_t *packData = paketlin a��laca�� verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketlenen dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		int16_t yemre;
 *  		uint8_t dizi[2];
 *  		dizi[0] = 0x00;
 *  		dizi[1] = 0x12
 *  		yemre = 0x1200;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT8toINT16_BE(int16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[1] = packBuf[*index];
	((uint8_t *)packData)[0] = packBuf[(*index) + 1];

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


/* Bu fonksiyon uint8_t t�r�nden veriyi int16_t olarak paketlemeye yarar.
 * Buradaki tasar�m Big Endian paketlenmi� veriyi birle�tirir.
 * Parametreler : uint16_t *packData = paketlin a��laca�� verinin adresi girilir.
 * 				: uint8_t  *packBuf  = paketlenen dizi girilir.
 * 				: uint16_t *index    = paketin indeks s�ras� girilir.
 * �RNEK:
 *  		uint16_t yemre;
 *  		uint8_t dizi[2];
 *  		dizi[0] = 0x00;
 *  		dizi[1] = 0x12
 *  		yemre = 0x0012;
 *  		fonksiyonun ��k��� yukar�da g�sterildi�i gibi olur.
 */
void AESK_UINT8toUINT16_BE(uint16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[1] = packBuf[*index];
	((uint8_t *)packData)[0] = packBuf[(*index) + 1];

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

void AESK_UINT8toINT32_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *) packData)[3] = packBuf[*index];
	((uint8_t *) packData)[2] = packBuf[(*index) + 1];
	((uint8_t *) packData)[1] = packBuf[(*index) + 2];
	((uint8_t *) packData)[0] = packBuf[(*index) + 3];

	*index = *index + sizeof(int32_t);
}
/***************************************  INT32_T ENCODE END  *********************************************************/

/***************************************  UINT32_T ENCODE START  *********************************************************/
void AESK_UINT8toUINT32_BE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[3] = packBuf[(*index)];
	((uint8_t*) packData)[2] = packBuf[(*index) + 1];
	((uint8_t*) packData)[1] = packBuf[(*index) + 2];
	((uint8_t*) packData)[0] = packBuf[(*index) + 3];

	*index = *index + sizeof(uint32_t);
}

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

void AESK_UINT8toFLOAT32_BE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[3] = packBuf[(*index)];
	((uint8_t*) packData)[2] = packBuf[(*index) + 1];
	((uint8_t*) packData)[1] = packBuf[(*index) + 2];
	((uint8_t*) packData)[0] = packBuf[(*index) + 3];

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

void AESK_UINT8toFLOAT64_BE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[7] = packBuf[(*index)];
	((uint8_t*) packData)[6] = packBuf[(*index) + 1];
	((uint8_t*) packData)[5] = packBuf[(*index) + 2];
	((uint8_t*) packData)[4] = packBuf[(*index) + 3];
	((uint8_t*) packData)[3] = packBuf[(*index) + 4];
	((uint8_t*) packData)[2] = packBuf[(*index) + 5];
	((uint8_t*) packData)[1] = packBuf[(*index) + 6];
	((uint8_t*) packData)[0] = packBuf[(*index) + 7];
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
void AESK_UINT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 uint16_t data ;
	((uint8_t *)&data)[1] = packBuf[*index];
	((uint8_t *)&data)[0] = packBuf[(*index) + 1];

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
void AESK_INT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 int16_t data ;
	((uint8_t *)&data)[1] = packBuf[*index];
	((uint8_t *)&data)[0] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;
	*index = *index + sizeof(int16_t);
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


void AESK_UINT64toUINT8_LE(uint64_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 4] = ((uint8_t*)packData)[4];
	packBuf[(*index) + 5] = ((uint8_t*)packData)[5];
	packBuf[(*index) + 6] = ((uint8_t*)packData)[6];
	packBuf[(*index) + 7] = ((uint8_t*)packData)[7];

	*index = *index + sizeof(uint64_t);
}

void AESK_UINT8toUINT8_LE(uint8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];

	*index = *index + sizeof(uint8_t);
}
