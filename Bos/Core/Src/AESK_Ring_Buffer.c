/*
 * AESK_Ring_Buffer.c
 *
 *  Created on: Nov 28, 2020
 *      Author: basri
 */
#include <AESK_Ring_Buffer.h>
/* Farkli Hzlerde calisan haberlesme yapilarinda siklikla kullanilan ringbuffer yapisi AESK kutuphanelerine entegre edilmistir.
 * Tail-head metodu baz alinarak yapilmistir. Ozellikle CAN gibi gonderilecek framein sabit ve az oldugu yapilarda interrupt yapilariyla kaymak
 * gibi kaymaktadir. Bircok algoritmada kolaylik saglamaktadir. Lutfen ring buffer yapisini iyi ogrenin.
 */


/*
 * Bufferimizi sifirliyoruz.Dunyaya tertemiz hic kirlenmeden geliyor. Her seyi 0.
 */
void Ring_Buffer_Init(AESK_Ring_Buffer* rng_buf)
{
	for(uint16_t i = 0; i < sizeof(AESK_Ring_Buffer); i++)
	{
		((uint8_t*)rng_buf)[i] = 0; // her baba yigidin harci degil sunu yazabilmek
	}
}

/*
 * Ring buffera veri yazar. Ilk parametre olusturulan ring bufferi talep etmektedir.
 * 2.parametrede yazilacak olan verinin adresi girilir.
 * 3.parametrede yazilacak olan verinin boyutu girilmektedir.
 */
void Write_Data_Ring_Buffer(	  AESK_Ring_Buffer* rng_buf,
							const uint8_t* tmp_buf,
							size_t size)
{
	for(uint8_t i = 0; i < size; i++)
	{
		if(rng_buf->remainder_byte < RING_BUFFER_SIZE) // Ring buffer boyutu kadar veri yigilmadiysa buffera veri eklenebilir.
		{
			rng_buf->remainder_byte++; // veriyi ekledik artik. yigini arttirmak lazim.
			rng_buf->ring_buffer[rng_buf->head] = tmp_buf[i]; // veriyi head son kaldigi yere yazalim.
			rng_buf->head = ((rng_buf->head + 1) % RING_BUFFER_SIZE); // head'i arttiralim ki ustune yazmak zorunda kalmayalim.
		}

		else
		{
			rng_buf->tail = ((rng_buf->tail + 1) % RING_BUFFER_SIZE); // veri yigilmis artik. yapacak bi sey yok. Taili arttirip yer acalim.
																	  // eski veriler gitti artik. yenileriyle yola devam.
		}
	}
}

/*
 * Ring bufferdan bir baytlik okuma yapar.
 */
int16_t Read_Byte_Ring_Buffer(AESK_Ring_Buffer* rng_buf, uint8_t* tmp_buf)
{
	uint16_t read_byte = 0;

	if(rng_buf->remainder_byte > 0)
	{
		*tmp_buf = rng_buf->ring_buffer[rng_buf->tail];
		read_byte++;
		rng_buf->tail = ((rng_buf->tail + 1) % RING_BUFFER_SIZE);
		rng_buf->remainder_byte--;
	}
	else
	{
		return read_byte;
	}

	return read_byte;
}

/*
 * Ring bufferda ne kadar veri varsa hepsini okur.
 */
int16_t Read_All_Buffer(AESK_Ring_Buffer* rng_buf, uint8_t* tmp_buf)
{
	int16_t idx = 0;

	while(rng_buf->remainder_byte)
	{
		Read_Byte_Ring_Buffer(rng_buf, &(tmp_buf[idx]));
		idx++;
	}
	return idx;
}


