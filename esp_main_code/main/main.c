#include "main.h"

static const char *TAG = "AESK";
static char *MOUNT_POINT = SD_MOUNT;

static EventGroupHandle_t flow_event_group;
EventGroupHandle_t xEventTask;
int FTP_TASK_FINISH_BIT = BIT2;
const static int CONNECTED_BIT = BIT0;


Telemetry_Packet aesk_data;

//Task Handles
TaskHandle_t uart_receive_t;
TaskHandle_t data_proc_t;
TaskHandle_t udp_telemetry_t;
TaskHandle_t tcp_command_t;
TaskHandle_t video_stream_t;
TaskHandle_t video_record_t;
TaskHandle_t file_transfer_t;




//Queues
QueueHandle_t telemetry_flow_data;
QueueHandle_t telemetry_flow_udp;

void init_wifi(wifi_mode_t mode);
esp_err_t mountSDCARD(char * mount_point, sdmmc_card_t * card);
bool parseAddress(int * ip, char * text);
void initialise_mdns(void);
void time_sync_notification_cb(struct timeval *tv);
static void initialize_sntp(void);
static esp_err_t obtain_time(void);
void ftp_task(void *pvParameters);

static camera_config_t camera_config = CAM_CONFIG_DEFAULT();

static void uart_rx_task(void *pvParameters){UartReceive_S();}
static void data_proc_task(void *pvParameters){DataProcess();}
static void udp_telemetry_task(void *pvParameters){TelemetryTransmit_UC();}
static void tcp_command_task(void *pvParameters){CommandListen_TC();}
static void udp_video_task(void *pvParameters){VideoStream_UC();}
static void video_record_task(void *pvParameters){VideoRecorder();}

//void ftp_task (void *pvParameters);

//Connection States
static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG,"CONNECTED TO AP WAITING IP...");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG,"GOT IP");
            printf("\nOur IP address is " IPSTR " ",IP2STR(&event->event_info.got_ip.ip_info.ip));
            xEventGroupSetBits(flow_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG,"DISCONNECTED FROM AP");
            esp_wifi_connect();
            xEventGroupClearBits(flow_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}


void app_main(void)
{
    //Flash Init
	esp_err_t ret;
	ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
	ESP_ERROR_CHECK(nvs_flash_erase());
	ret = nvs_flash_init();
	}
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    //! ST Kalıcı Verileri Okuma
    reset_counter = 0;
    aesk_data.team_number = 0x93C9;
    aesk_data.packet_number =0;
    test_init_variable(&aesk_data);

    nvs_handle_t my_handle;
    int err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if(err!=ESP_OK)
    {
    }
    else//*EPROM READ BASARILI
    {
        nvs_get_u16(my_handle,"esp_status",&aesk_data.esp_status.AsNumber);
        nvs_get_u16(my_handle,"stm_status",&aesk_data.stm_status.AsNumber);
        if(aesk_data.esp_status.Flags.x8_ar_full_reset)
        {
            //? burada bişey yapmasına gerek var mı??
            aesk_data.esp_status.AsNumber =0x0000;
        }
        else
        {
            if(!(aesk_data.esp_status.Flags.x9_ar_synch_time))
            {
                //* Burada kalıcı verilerden Zamanı çekecek
                nvs_get_u32(my_handle,"transmit_time",&aesk_data.transmit_time);
                //* Sistem saatini eşleştirdi
                struct timeval tv;
                tv.tv_sec = aesk_data.transmit_time;
                settimeofday(&tv,NULL);
            }
            if(!(aesk_data.esp_status.Flags.xb_ar_reset_counters)){
                //* Default 0
                nvs_get_u8(my_handle,"reset_counter",&reset_counter);
                nvs_get_u16(my_handle,"packet_counter",&aesk_data.packet_number);
            }
        }

    reset_counter++;
    if(reset_counter>254){
        reset_counter =0;
    }
    nvs_set_u8(my_handle,"reset_counter",reset_counter);
    reset_counter--;

    nvs_commit(my_handle);
    nvs_close(my_handle);
    }
    
    telemetry_flow_data = xQueueCreate(5,TELEMETRY_PACKET_SIZE);
    telemetry_flow_udp = xQueueCreate(5,TELEMETRY_PACKET_SIZE);
    aesk_data.team_number = 0x93C9;
     //Init Cam, Output Format etc...
    #if CAM_ACTIVE
    while(init_camera(&camera_config)!=ESP_OK){
    	vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    #endif
      ESP_LOGE("MAIN", "1");
      init_wifi(WIFI_MODE_STA);
      ESP_LOGE("MAIN", "2");

    #if SET_MDNS
    initialise_mdns();
    #endif


	ret = obtain_time();
	if(ret != ESP_OK) {
		ESP_LOGE(TAG, "Fail to getting time over NTP.");
	}

    #if UART_ACTIVE
    //Uart Init
    UartInit();
    #endif

   //  //To mount sd card, global_configs -> SD_CARD_ACTIVE 1
   //  //also change the uart pins to 1&3, disable uart debug output in sdkconfigs
    #if SD_CARD_ACTIVE
    sdmmc_card_t card;
	ret = mountSDCARD(MOUNT_POINT, &card);
    #endif

	#if SD_CARD_ACTIVE
    FILE *file = fopen(SD_MOUNT"/log.csv", "a");
    if (file != NULL)
    {
    ESP_LOGI("LOG", "Task File Open");
    fprintf(file,"\nTeam Number,Packet Number,Date,Time,Pressure,Altitude,Velocity,Temperature,Battery Voltage,GPS LAT,GPS LONG,GPS ALT,Satellite State,Pitch,Roll,Yaw,Spin Counter,Motor Power,RSSI,Last Command,Command State\n");
    }
    else
    {
    ESP_LOGE("LOG", "Could not open file Task");
    }
    fclose(file);
	#endif

    #if UART_ACTIVE
    //Starts tasks by one by
    xTaskCreate(uart_rx_task,       "Uart Receive Task",  4*ONE_KB,        NULL ,configMAX_PRIORITIES-3,  &uart_receive_t);
    #endif

    xTaskCreate(data_proc_task,     "Data Process Task",  8*ONE_KB,        NULL ,configMAX_PRIORITIES-4,  &data_proc_t);
    xTaskCreate(udp_telemetry_task, "Telemetry Tx Task",  4*ONE_KB,        NULL ,configMAX_PRIORITIES-3,  &udp_telemetry_t);

    xTaskCreate(tcp_command_task,   "Command Listen",     4*ONE_KB,        NULL ,configMAX_PRIORITIES-5,  NULL);

    #if CAM_ACTIVE
    ESP_LOGE("VIDEO","VIDEO STREAM HAS STARTED");
    xTaskCreate(udp_video_task,"Video Stream Task",VIDEO_TASK_SIZE, NULL ,configMAX_PRIORITIES-5,&video_stream_t);
    #endif

	#if SD_CARD_ACTIVE
    xTaskCreate(video_record_task,"Video Record", VIDEO_TASK_SIZE, NULL ,configMAX_PRIORITIES-4,  NULL);
    xTaskCreate(ftp_task,"FTP Task",12*ONE_KB,NULL,configMAX_PRIORITIES-6, &file_transfer_t);
    vTaskSuspend(file_transfer_t);
    #endif

}
/*Functions that used in main*/

void init_wifi(wifi_mode_t mode)
{
	//ESP_ERROR_CHECK(esp_netif_init());
    tcpip_adapter_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode)) ;
    flow_event_group = xEventGroupCreate();

    if (mode == WIFI_MODE_STA) {
        wifi_config_t config = {
            .sta = {
                .ssid = WIFI_SSID,
                .password = WIFI_PASSWORD,
                .bssid_set = false
            }
        };
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &config) );
        ESP_ERROR_CHECK( esp_wifi_start() );
        ESP_ERROR_CHECK( esp_wifi_connect() );
        xEventGroupWaitBits(flow_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
        ESP_LOGI(TAG, "Connected to AP");
    }
    else
    {
        wifi_config_t config = {
            .ap = {
                .ssid = WIFI_SSID,
                .password = WIFI_PASSWORD,
                .ssid_len = 0,
                .authmode = WIFI_AUTH_WPA_WPA2_PSK,
                .ssid_hidden = false,
                .max_connection = 3,
                .beacon_interval = 100,
            }
        };
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &config) );
        ESP_ERROR_CHECK( esp_wifi_start() );
    }
}

esp_err_t mountSDCARD(char * mount_point, sdmmc_card_t * card) {
	esp_err_t ret;
	// Options for mounting the filesystem.
	// If format_if_mount_failed is set to true, SD card will be partitioned and
	// formatted in case when mounting fails.
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
		.format_if_mount_failed = true,
		.max_files = MAX_OPEN_FILES,
		.allocation_unit_size = ALLOCATION_UNIT_SIZE
	};
	//sdmmc_card_t* card;

	ESP_LOGI("SD MOUNT", "Initializing SDMMC peripheral");
	sdmmc_host_t host = SDMMC_HOST_DEFAULT();
	// This initializes the slot without card detect (CD) and write protect (WP) signals.
	// Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
	sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
	// To use 1-line SD mode, uncomment the following line:
	// slot_config.width = 1;
	// GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
	// Internal pull-ups are not sufficient. However, enabling internal pull-ups
	// does make a difference some boards, so we do that here.
	gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);	// CMD, needed in 4- and 1- line modes
	gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);	// D0, needed in 4- and 1-line modes
	gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);	// D1, needed in 4-line mode only
	gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);	// D2, needed in 4-line mode only
	gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);	// D3, needed in 4- and 1-line modes
	ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

	ESP_LOGI("SD MOUNT", "Mounte SD card on %s", mount_point);

	return ret;
}

bool parseAddress(int * ip, char * text) {
	int len = strlen(text);
	int octet = 0;
	char buf[4];
	int index = 0;
	for(int i=0;i<len;i++)
	{
		char c = text[i];
		if (c == '.')
		{
			ip[octet] = strtol(buf, NULL, 10);
			octet++;
			index = 0;
		}
		else
		{
			if (index == 3) return false;
			if (c < '0' || c > '9') return false;
			buf[index++] = c;
			buf[index] = 0;
		}
	}
	if (strlen(buf) > 0) {
		ip[octet] = strtol(buf, NULL, 10);
		octet++;
	}
	if (octet != 4) return false;
	return true;
}

void initialise_mdns(void)
{
	//initialize mDNS
	ESP_ERROR_CHECK( mdns_init() );
	//set mDNS hostname (required if you want to advertise services)
	ESP_ERROR_CHECK( mdns_hostname_set(ESP_MDNS_HOSTNAME) );
	ESP_LOGI("MDNS", "mdns hostname set to: [%s]", ESP_MDNS_HOSTNAME);
}

void time_sync_notification_cb(struct timeval *tv)
{
	ESP_LOGI("TIME", "Notification of a time synchronization event");
	//aesk_data.esp_status.Flags.x9_ar_synch_time =0;
}

static void initialize_sntp(void)
{
	ESP_LOGI("SNTP", "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	//sntp_setservername(0, "pool.ntp.org");
	ESP_LOGI("SNTP", "Your NTP Server is %s", NTP_SERVER);
	sntp_setservername(0, NTP_SERVER);
	sntp_set_time_sync_notification_cb(time_sync_notification_cb);
	sntp_init();
}

static esp_err_t obtain_time(void)
{
	initialize_sntp();
	int retry = 0;
	const int retry_count = 10;
	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
		ESP_LOGI("TIME", "Waiting for system time to be set... (%d/%d)", retry, retry_count);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	if (retry == retry_count) return ESP_FAIL;
	return ESP_OK;
}
