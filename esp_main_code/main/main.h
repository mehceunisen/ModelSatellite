/*
 * main.h
 *
 *  Created on: 23 Jun 2021
 *      Author: afunl
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sys/unistd.h>
#include <math.h>

#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_camera.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "mdns.h"
#include "esp_sntp.h"
#include "lwip/dns.h"

#include "global_configs.h"
#include "af_camera_video.h"
#include "aesk_data_flow.h"
#include "aesk_tasks.h"

#endif /* MAIN_MAIN_H_ */
