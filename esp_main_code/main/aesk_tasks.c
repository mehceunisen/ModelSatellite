#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_camera.h"

#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "aesk_tasks.h"
#include "global_configs.h"
#include "aesk_data_flow.h"
#include "af_camera_video.h"


extern Telemetry_Packet aesk_data;


extern QueueHandle_t telemetry_flow_data;
extern QueueHandle_t telemetry_flow_udp;

//Semaphore Handler
SemaphoreHandle_t videoSemaphore = NULL;

//Task Handles
//Task Handles
extern TaskHandle_t uart_receive_t;
extern TaskHandle_t data_proc_t;
extern TaskHandle_t udp_telemetry_t;
//extern TaskHandle_t tcp_command_t;
extern TaskHandle_t video_stream_t;
extern TaskHandle_t video_record_t;
extern TaskHandle_t file_transfer_t;


void UartInit(void){
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int uart_transmit(const char* data,uint8_t size)
{
    const int txBytes = uart_write_bytes(UART_NUM_1, data, size);
    return txBytes;
}

void CommandExecute(uint8_t *command_buffer, uint8_t len) {
	//burada bol bol if kullanılacak
	printf("\n");
	for(int i=0;i<len;i++){
		printf(" %d",command_buffer[i]);
	}
	printf("\n");
	if (command_buffer[0] == ESP_COMMAND_H0 && command_buffer[1] == ESP_COMMAND_H1) {
		bool is_task_exist=1;

		aesk_data.last_command_code = command_buffer[2];
		aesk_data.last_command_state = 1;

		//ESP32 ye gelen komutlar, burada nested ifler olacak
		if (command_buffer[2] == ESP_RESET) {
			fflush(stdout);
			esp_restart();
			aesk_data.last_command_state = 0;

		}
		if (command_buffer[2] == FULL_RESET) {
			aesk_data.esp_status.AsNumber =0;
			fflush(stdout);
			esp_restart();
			aesk_data.last_command_state = 0;
		}
		if (command_buffer[2] == SET_MATRIX) {
			if (command_buffer[3] == STM_MATRIX_ID) {
				aesk_data.stm_status.AsNumber = getu16_from2u8(command_buffer[3],command_buffer[4]);
			}
			else if (command_buffer[2] == ESP_MATRIX_ID) {
				aesk_data.esp_status.AsNumber = getu16_from2u8(command_buffer[3],command_buffer[4]);;
			}
			else {

			}
		}
		if (command_buffer[2] == SYNC_TIME)
		{
			//Burada Programın akışını bozmayacak şekilde zaman eşleştirmesi yapılacak
		}
		if (command_buffer[2] == SET_TEAM_NUMBER) {
			aesk_data.team_number = getu16_from2u8(command_buffer[3],command_buffer[4]);;
		}

		TaskHandle_t* temp_task_h;
		if(command_buffer[2] == SET_TASK){
			switch (command_buffer[3]) {
				case UART_TASK_N:
					temp_task_h = &uart_receive_t;
					break;
				case DATA_TASK_N:
					temp_task_h = &data_proc_t;
					break;
				case TELEMERTY_TASK_N:
					temp_task_h = &udp_telemetry_t;
					break;
				case VSTREAM_TASK_N:
					temp_task_h = &video_stream_t;
					break;
				case VRECORD_TASK_N:
					temp_task_h = &video_record_t;
                    is_recording = false;
					break;
				case FTP_TASK_N:
					temp_task_h = &file_transfer_t;
					break;
				default:
					is_task_exist = 0;
					break;
			}

			if(is_task_exist){
				//checking task created or not
				printf("\n Task name %d",is_task_exist);
				if(*temp_task_h !=NULL){
					printf("\n Buraya da girdi");
					eTaskState temp_state = eTaskGetState(*temp_task_h);
					//always check status
					if(command_buffer[4]==TASK_SUSPEND && (temp_state != eSuspended)&&(temp_state)){
						printf("\n Task Has Been Suspended");
						vTaskSuspend(*temp_task_h);
					}
					else if(command_buffer[4]==TASK_RESUME && (temp_state == eSuspended)){
						printf("\n Task Resumed");
						vTaskResume(*temp_task_h);
					}
					else
					{
						//handle non protocol commands
						printf("\n Buraya girmese olurdu");
						aesk_data.last_command_state = 0;
					}
				}
				else
				{
					//task not created
				}
				//free(temp_task_h);
			}
			else{
				aesk_data.last_command_state =0;
			}
		}

	}
	else if (command_buffer[0] == STM_COMMAND_H0 && command_buffer[1] == STM_COMMAND_H1) {
		//Direkt uart transmit yap stm32 ye
		//leni parametre olarak alıyoruz, tcp_rx yaptıktan sonra
		//leni parametre olarak veriyoruz burası önemli
		#if UART_ACTIVE
		uart_transmit((char*)command_buffer,len);
		#endif
	}
	else
	{
		//Yanlıs Komut geldi
	}
}


void UartReceive_S(void){
    
    uint8_t* uart_received_data = (uint8_t*) malloc((RX_BUF_SIZE/2)+1);
    uint8_t* our_packet =(uint8_t*)malloc(DATA_LEN+1);
    //500 lük buffer olan pool tanımla buraya,
    //bir de bu poolu queue ile ver , orada işlenip değişkenler atansın
    while (1) {
        /*
        uart_read_bytes, içindeki 2. ve 3 . parametrelere göre ringbuffer içeriğini adresi verilen buffere yazar
        ve kaç byte okuduğunu döndürür
        */
        uint16_t crc = 0;
        const int rxBytes = uart_read_bytes(UART_NUM_1, uart_received_data, RING_BUFFER_READ_SIZE ,(1000/READ_FREQ_HZ)/portTICK_PERIOD_MS);
        uint8_t  data_counter = 0;
        uint8_t  compose_states = catch_header_1;
        uint8_t data_len = 0;
        if (rxBytes > 0) {
            for(int i=0;i<rxBytes;i++)
            {
                switch (compose_states)
                {
                case catch_header_1:
                    if(uart_received_data[i]==HEADER_1)
                    {
                        compose_states = catch_header_2;
                    }
                    else
                    {
                        compose_states = catch_header_1;
                    }
                    break;
                case catch_header_2:
                    if(uart_received_data[i]==HEADER_2)
                    {
                        compose_states = get_len;
                    }
                    else
                    {
                        compose_states = catch_header_1;
                    }
                    break;
                case get_len:
                    data_counter =0;
                    data_len = uart_received_data[i];
                    compose_states = add_buffer;
                    break;
                case add_buffer:
                    our_packet[data_counter++] = uart_received_data[i];
                    if(data_counter >= data_len)
                    {
                        compose_states = crc_l_check;
                    }
                    break;
                case crc_l_check:
                    crc = uart_received_data[i];
                    compose_states = crc_h_check;
                    break;
                case crc_h_check:
                    crc = crc | (uart_received_data[i] << 8);
                    if(aeskCRCCalculator(our_packet,data_len) == crc )
                    {
                    	//Burada Buffer Güncellenir.
                        xQueueSendToFront(telemetry_flow_data,our_packet,0);
                        compose_states = catch_header_1;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        
    }
    free(uart_received_data);
    free(our_packet);
}

void DataProcess(void){
    
    uint8_t* our_packet =(uint8_t*)malloc(DATA_LEN+1);
    
    char log_text[100];

    nvs_handle_t my_handle;
    while(1)
    {
        //veri gelmedigi surece kod burada durmali
		#if UART_ACTIVE
        xQueueReceive(telemetry_flow_data,our_packet,portMAX_DELAY); 
		#else
        xQueueReceive(telemetry_flow_data,our_packet,1000/portTICK_PERIOD_MS);
		#endif
        uint16_t index =0;

        /*Her pakette ototmatik olarak değişecek verileri. Packet Counter,*/
        aesk_data.packet_number ++;

        //sistemimiz yerel zamanda çalışmıyor ama yer istasyonuna yerel zaman gönderilecek
        aesk_data.transmit_time=(uint32_t)time(NULL);
        aesk_data.transmit_time+=(LOCAL_TIMEZONE*60*60);
        
        //bu değişmeyecek ama yine de paket devamlılığı için yazılacak
        AESK_UINT16toUINT8_LE(&aesk_data.team_number,our_packet,&index);
        AESK_UINT16toUINT8_LE(&aesk_data.packet_number,our_packet,&index);
        AESK_UINT32toUINT8_LE(&aesk_data.transmit_time,our_packet,&index);
        
        //paketin çözülüp değişkenlere atanması
        AESK_UINT8toUINT16_LE(&aesk_data.press,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.altitute_m,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.velocity_mds,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.temperature,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.battery_voltage,our_packet,&index);
        AESK_UINT8toUINT32_LE(&aesk_data.gps_lat,our_packet,&index);
        AESK_UINT8toUINT32_LE(&aesk_data.gps_long,our_packet,&index);
        AESK_UINT8toUINT32_LE(&aesk_data.gps_alt,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.stm_status.AsNumber,our_packet,&index);

        //ESP Status
        AESK_UINT16toUINT8_LE(&aesk_data.esp_status.AsNumber,our_packet,&index);

        AESK_UINT8toUINT16_LE(&aesk_data.pitch,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.roll,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.yaw,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.roll_counter,our_packet,&index);
        AESK_UINT8toUINT16_LE(&aesk_data.motor_power,our_packet,&index);
            
        wifi_ap_record_t ax;
        esp_err_t x = esp_wifi_sta_get_ap_info(&ax);
        if(x==ESP_OK){
        	aesk_data.rssi = ax.rssi;
            AESK_INT8toUINT8CODE(&aesk_data.rssi,our_packet,&index);
        }
        else{
        	aesk_data.rssi =0;
        	index++;
        }
        AESK_UINT8toUINT8CODE(&aesk_data.last_command_code,our_packet,&index);
        AESK_UINT8toUINT8CODE(&aesk_data.last_command_state,our_packet,&index);
        //Güncellenmiş telemetri paketi TCP taskına gönderildi
        
        xQueueSendToFront(telemetry_flow_udp,our_packet,0);
        //to log out datas to .csv file 
        #if SD_CARD_ACTIVE 
        log_string_g(log_text,aesk_data);
        printf("\n%s",log_text);
        FILE *file = fopen(SD_MOUNT"/log.csv", "a");
        if (file != NULL)
        {
        ESP_LOGI("LOG", "Task File Open");
        int koko = fprintf(file,"%s\n",log_text);
        if(koko){
        	ESP_LOGI("TAG","FILE ACTI");
        }
        else
        {
        	ESP_LOGW("TAG","FILE PATLADI");
        }
        }
        else
        {
        ESP_LOGE("LOG", "Could not open file Task");
        }
        fclose(file);
        #endif

        //kalıcı verileri güncelleştir
        //kalıcı verileri flash memory'e tekrardan yaz, kalıcı veriler uygulama başlatılınca bir kereliğine memoryden çekilir.
        int err_x = nvs_open("storage", NVS_READWRITE, &my_handle);
        if(err_x ==ESP_OK){
        nvs_set_u16(my_handle,"stm_status",aesk_data.stm_status.AsNumber);
        nvs_set_u16(my_handle,"esp_status",aesk_data.esp_status.AsNumber);
        nvs_set_u16(my_handle,"packet_counter",aesk_data.packet_number);
        nvs_set_u32(my_handle,"transmit_time",aesk_data.transmit_time);
        //nvs_set_u16(my_handle,"roll_coutner",aesk.roll_counter);
        nvs_commit(my_handle);
        nvs_close(my_handle);
        }

		#if UART_ACTIVE
        vTaskDelay(850/portTICK_PERIOD_MS);
		#endif

    }

}

void TelemetryTransmit_UC(){
    
    int addr_family = 0;
    int ip_protocol = 0;
    uint8_t* our_packet =(uint8_t*)malloc(DATA_LEN+1);
    while(1){
        while (1) {

            struct sockaddr_in dest_addr;
            dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_port = htons(UDP_TX_PORT);
            addr_family = AF_INET;
            ip_protocol = IPPROTO_IP;
            
            int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
            
            if (sock < 0) {
            ESP_LOGE("Telemetry TX", "Unable to create socket: errno %d", errno);
            break;
            }
            
            while (1) {
                xQueueReceive(telemetry_flow_udp,our_packet,(1000/TX_FREQ_HZ)/portTICK_PERIOD_MS);
                sendto(sock,our_packet,DATA_LEN+1, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            }
            if (sock != -1) {shutdown(sock, 0);close(sock);}
        }
    }
}


void CommandListen_TC(void){
      //TCP receive Buffer
    char rx_buffer[COMMAND_PACKET_SIZE];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while(1){
    while (1) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(TCP_RX_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);

        if (sock < 0) {break;}

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));

        if (err != 0) {shutdown(sock, 0);close(sock);break;}

        ESP_LOGI("COMMAND","RECEIVE BAGLANTI");
        //Bağlantı durumu var, durum matrisini güncelle

        while (1) {
            //bağlantıdan mesaj gelene kadar bekler ,kodu orada tutar
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0)
			{
                ESP_LOGE("COMMAND", "RX recv failed: errno %d", errno);
                break;
            }
            else
			{
            //buralar test edilsin , veri basılarak, string ile olmayabilir.
            CommandExecute((uint8_t*)rx_buffer, len);
            }
        }
        if (sock != -1) {ESP_LOGE("COMMAND", "RX Disconnected");shutdown(sock, 0);close(sock);break;}
    }
	}
} 

void VideoStream_UC(void){

    int addr_family = 0;
    int ip_protocol = 0;

    uint8_t frame_chunk[CHUNK_SIZE+CHUNK_HEADER_SIZE];
    uint16_t chunk_count=0;
    videoSemaphore = xSemaphoreCreateBinary();
    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(VIDEO_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        static uint16_t frame_number=0;
        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            break;
        }
        while (1) {
            //frame periot
            vTaskDelay(MS_DELAY_BETWEEN_CHUNKS / portTICK_PERIOD_MS);        

            //getting frame
            camera_fb_t *pic = esp_camera_fb_get();
            xSemaphoreGive(videoSemaphore);
            //chunk count
            chunk_count = (pic->len + CHUNK_SIZE - 1) / CHUNK_SIZE;

            for(uint8_t chunk_number=0; chunk_number<(chunk_count-1); chunk_number++)
            {
                SplitIntoChunks(frame_chunk,pic->buf,
                (chunk_number*CHUNK_SIZE),
                ((chunk_number+1)*CHUNK_SIZE), &frame_number,chunk_number,&chunk_count);
                sendto(sock, frame_chunk, sizeof(frame_chunk), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                vTaskDelay(MS_DELAY_BETWEEN_CHUNKS / portTICK_PERIOD_MS);
            }
            uint16_t _remainderTemp = (pic->len) % CHUNK_SIZE;
            SplitIntoChunks(frame_chunk,pic->buf,((chunk_count-1)*CHUNK_SIZE),(((chunk_count-1)*CHUNK_SIZE)+_remainderTemp+CHUNK_HEADER_SIZE), &frame_number,chunk_count-1,&chunk_count);
            for(uint16_t i=_remainderTemp+5;i<CHUNK_SIZE;i++){

                frame_chunk[i]=(uint8_t)65;
            }
            sendto(sock, frame_chunk, sizeof(frame_chunk), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            frame_number++;
            esp_camera_fb_return(pic);
        }
    }
}

void VideoRecorder(){
    while(1){
        if(xSemaphoreTake(videoSemaphore, portMAX_DELAY)){
            record_video();
        }      
    }
}