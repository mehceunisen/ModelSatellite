#ifndef AF_CAMERA_VIDEO_H
#define AF_CAMERA_VIDEO_H

#include "esp_camera.h"
#include "global_configs.h"
#include "esp_log.h"
#include "pgm.h"

#define CAM_PIX_FORMAT PIXFORMAT_JPEG
#define CAM_FRAME_SIZE FRAMESIZE_HVGA
//#define CAM_FRAME_SIZE FRAMESIZE_CIF
#define CAM_JPEG_QUALITY 15
#define CAM_XLCK_FREQ 20000000
#define CAM_FB_COUNT 2


//Camera Pins For Ai Thinker Esp32-Cam
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27
#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

//Replacing
#define CAM_CONFIG_DEFAULT(){\
    .pin_pwdn = CAM_PIN_PWDN,\
    .pin_reset = CAM_PIN_RESET,\
    .pin_xclk = CAM_PIN_XCLK,\
    .pin_sscb_sda = CAM_PIN_SIOD,\
    .pin_sscb_scl = CAM_PIN_SIOC,\
    .pin_d7 = CAM_PIN_D7,\
    .pin_d6 = CAM_PIN_D6,\
    .pin_d5 = CAM_PIN_D5,\
    .pin_d4 = CAM_PIN_D4,\
    .pin_d3 = CAM_PIN_D3,\
    .pin_d2 = CAM_PIN_D2,\
    .pin_d1 = CAM_PIN_D1,\
    .pin_d0 = CAM_PIN_D0,\
    .pin_vsync = CAM_PIN_VSYNC,\
    .pin_href = CAM_PIN_HREF,\
    .pin_pclk = CAM_PIN_PCLK,\
    .xclk_freq_hz = CAM_XLCK_FREQ,\
    .ledc_timer = LEDC_TIMER_0,\
    .ledc_channel = LEDC_CHANNEL_0,\
    .pixel_format = CAM_PIX_FORMAT,\
    .frame_size =CAM_FRAME_SIZE ,\
    .jpeg_quality = CAM_JPEG_QUALITY,\
    .fb_count = CAM_FB_COUNT \
}

esp_err_t init_camera(camera_config_t* cam_config);

/*
Usage : 
*/
void SplitIntoChunks(uint8_t* chunk_buffer ,
                    uint8_t* framebuffer, 
                    int start_index, 
                    int end_index, 
                    uint16_t* frameNumber, 
                    uint8_t chunkNumber,
                    uint16_t* chunk_count);


uint32_t millis();

void inline print_quartet(unsigned long i, FILE *fd);

void inline print_2quartet(unsigned long i, unsigned long j, FILE *fd);

void major_fail();

camera_fb_t *get_good_jpeg();

void start_avi();

void another_save_avi(camera_fb_t *fb);

void end_avi();

void record_video();

#endif
