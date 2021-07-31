#ifndef GLOBAL_CONFIGS_H
#define GLOBAL_CONFIGS_H

#include "esp_log.h"

#define OR_ ||
#define AND_ &&
#define ONE_KB 1024

#define LED 33

//Wifi Auth
//#define WIFI_SSID "ARMUT"
#define WIFI_SSID "mehceuny"
#define WIFI_PASSWORD "csustasi"

//Developing 
#define IS_DEBUG 1

//MDNS 
#define SET_MDNS 0
#define ESP_MDNS_HOSTNAME "aesk_sat"

//FTP Server
#define FTP_AUTH_USERNAME "digital"
#define FTP_AUTH_PASSWORD "aesk"
#define FTP_TASK_SIZE 1024*6

//IP of PC
//#define HOST_IP_ADDR "192.168.0.100"
//#define HOST_IP_ADDR "192.168.43.248"
//#define HOST_IP_ADDR "192.168.43.45"
#define HOST_IP_ADDR "192.168.43.198"

//VIDEO STREAM SETTINGS
#define VIDEO_PORT 8888
#define CHUNK_SIZE 1000
#define CHUNK_HEADER_SIZE 5
#define MS_DELAY_BETWEEN_CHUNKS 10
#define PHOTO_FREQUENCY 5
#define VIDEO_TASK_SIZE 32768


/* READ UART & TELEMETRY DATAS, BUFFERS CONFIGS*/ 
#define DATA_LEN 47
#define HEADER 3
#define CRC_LEN 2
#define TELEMETRY_PACKET_SIZE ((DATA_LEN + HEADER + CRC_LEN))
#define RING_BUFFER_READ_SIZE 100
//1000/READ_FREQ_HZ must be an integer
#define READ_FREQ_HZ 20
uint8_t reset_counter;

//Command
#define COMMAND_PACKET_SIZE 50

//Protocols & Ports
#define UDP_TX_PORT 8080
#define TCP_RX_PORT 3333
#define TX_FREQ_HZ 4

//NTP
#define NTP_SERVER HOST_IP_ADDR
#define LOCAL_TIMEZONE 3

//SD MMC
#define SD_CARD_ACTIVE 1
#define CAM_ACTIVE 1
#define SD_MOUNT "/root"
#define FORMAT_IF_MOUNT_FAIL true
#define MAX_OPEN_FILES 8
#define ALLOCATION_UNIT_SIZE 16 * 1024

//Uart Pins
#define UART_ACTIVE 0
#if SD_CARD_ACTIVE
#define TX_PIN 1
#define RX_PIN 3
#define VIDEO_RECORD 0
#else
#define TX_PIN 13
#define RX_PIN 15
#define VIDEO_RECORD 0
#endif

#define RX_BUF_SIZE 1024  


//video kayıt bokları
#define FPS 10
#define FRAME_COUNT 100
bool is_recording;
#define RECORD_DELAY 0
//video kayıt bokları


#endif
