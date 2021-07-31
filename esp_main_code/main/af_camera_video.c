#include "af_camera_video.h"
#include "esp_log.h"
#include "math.h"
#include "pgm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global_configs.h"
extern TaskHandle_t video_record_t;
int framesize = 6;   
int quality = 20;    
int avi_length = 2400; 
int MagicNumber = 11;


camera_fb_t *fb_curr = NULL;
camera_fb_t *fb_next = NULL;

FILE *avifile = NULL;
FILE *idxfile = NULL;

long bp;
long ap;
long bw;
long aw;

int diskspeed = 0;
char fname[100];

static int i = 0;
uint8_t temp = 0, temp_last = 0;
unsigned long fileposition = 0;
uint16_t frame_cnt = 0;
uint16_t remnant = 0;
uint32_t length = 0;
uint32_t startms;
uint32_t elapsedms;
uint32_t uVideoLen = 0;
bool is_header = false;
int bad_jpg = 0;
int extend_jpg = 0;
int normal_jpg = 0;

int file_number = 0;
int file_group = 0;
long boot_time = 0;

long totalp;
long totalw;
float avgp;
float avgw;
bool isFinished = false;

int first = 1;
int frames = 0;
long frame_start = 0;
long frame_end = 0;
long frame_total = 0;
long frame_average = 0;
long loop_average = 0;
long loop_total = 0;
long total_frame_data = 0;
long last_frame_length = 0;
int done = 0;
long avi_start_time = 0;
long avi_end_time = 0;
int stop = 0;
int we_are_already_stopped = 0;

#define BUFFSIZE 512

uint8_t buf[BUFFSIZE];

#define AVIOFFSET 240 // AVI main header length

unsigned long movi_size = 0;
unsigned long jpeg_size = 0;
unsigned long idx_offset = 0;

uint8_t zero_buf[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t dc_buf[4] = {0x30, 0x30, 0x64, 0x63}; // "00dc"
uint8_t dc_and_zero_buf[8] = {0x30, 0x30, 0x64, 0x63, 0x00, 0x00, 0x00, 0x00};

uint8_t avi1_buf[4] = {0x41, 0x56, 0x49, 0x31}; // "AVI1"
uint8_t idx1_buf[4] = {0x69, 0x64, 0x78, 0x31}; // "idx1"

uint8_t vga_w[2] = {0x80, 0x02};  // 640
uint8_t vga_h[2] = {0xE0, 0x01};  // 480
uint8_t cif_w[2] = {0x90, 0x01};  // 400
uint8_t cif_h[2] = {0x28, 0x01};  // 296
uint8_t svga_w[2] = {0x20, 0x03}; // 800
uint8_t svga_h[2] = {0x58, 0x02}; // 600
uint8_t sxga_w[2] = {0x00, 0x05}; // 1280
uint8_t sxga_h[2] = {0x00, 0x04}; // 1024
uint8_t uxga_w[2] = {0x40, 0x06}; // 1600
uint8_t uxga_h[2] = {0xB0, 0x04}; // 1200

const int avi_header[AVIOFFSET] PROGMEM = {
  0x52, 0x49, 0x46, 0x46, 0xD8, 0x01, 0x0E, 0x00, 0x41, 0x56, 0x49, 0x20, 0x4C, 0x49, 0x53, 0x54,
  0xD0, 0x00, 0x00, 0x00, 0x68, 0x64, 0x72, 0x6C, 0x61, 0x76, 0x69, 0x68, 0x38, 0x00, 0x00, 0x00,
  0xA0, 0x86, 0x01, 0x00, 0x80, 0x66, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x02, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x84, 0x00, 0x00, 0x00,
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x76, 0x69, 0x64, 0x73,
  0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
  0x01, 0x00, 0x18, 0x00, 0x4D, 0x4A, 0x50, 0x47, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x4E, 0x46, 0x4F,
  0x10, 0x00, 0x00, 0x00, 0x6A, 0x61, 0x6D, 0x65, 0x73, 0x7A, 0x61, 0x68, 0x61, 0x72, 0x79, 0x67,
  0x6D, 0x61, 0x69, 0x6C, 0x4C, 0x49, 0x53, 0x54, 0x00, 0x01, 0x0E, 0x00, 0x6D, 0x6F, 0x76, 0x69,
};


esp_err_t init_camera(camera_config_t* cam_config)
{
    //initialize the camera
    esp_err_t err = esp_camera_init(cam_config);
    if (err != ESP_OK)
    {
        return err;
    }
    
    sensor_t * ss = esp_camera_sensor_get();
    ss->set_quality(ss, quality);
    ss->set_framesize(ss, (framesize_t)framesize);

    ss->set_brightness(ss, 1);  //up the blightness just a bit
    ss->set_saturation(ss, -2); //lower the saturation
    ESP_LOGE("CAMERA INIT", "CAMERA INITIALIZED");
    return ESP_OK;
}

void SplitIntoChunks(uint8_t* chunk_buffer ,uint8_t* framebuffer, int start_index, int end_index, uint16_t* frameNumber, uint8_t chunkNumber,uint16_t* chunk_count)
{
    //little endian
    chunk_buffer[0]=(uint8_t)(chunk_count[0] & 0xFF);
    chunk_buffer[1]=(uint8_t)((chunk_count[0]>>8) & 0xFF);
    chunk_buffer[2]=(uint8_t)(frameNumber[0] & 0xFF);
    chunk_buffer[3]=(uint8_t)((frameNumber[0]>>8) & 0xFF);
    chunk_buffer[4]=chunkNumber;


    for(int i=CHUNK_HEADER_SIZE,j=start_index;j<end_index;i++,j++){
        chunk_buffer[i] = framebuffer[j];
    }
    
}

uint32_t millis()
{
  uint32_t time = esp_timer_get_time();
  return round(time / 1000);
}

void inline print_quartet(unsigned long i, FILE *fd)
{
  uint8_t y[4];

  y[0] = i % 0x100;
  y[1] = (i >> 8) % 0x100;
  y[2] = (i >> 16) % 0x100;
  y[3] = (i >> 24) % 0x100;

  fwrite(y, 1, 4, fd);
}

void inline print_2quartet(unsigned long i, unsigned long j, FILE *fd)
{
  uint8_t y[8];

  y[0] = i % 0x100;
  y[1] = (i >> 8) % 0x100;
  y[2] = (i >> 16) % 0x100;
  y[3] = (i >> 24) % 0x100;
  y[4] = j % 0x100;
  y[5] = (j >> 8) % 0x100;
  y[6] = (j >> 16) % 0x100;
  y[7] = (j >> 24) % 0x100;

  fwrite(y, 1, 8, fd);
}

void major_fail()
{

  for (int i = 0; i < 10; i++)
  { // 10 loops or about 100 seconds then reboot
    for (int j = 0; j < 3; j++)
    {
    }

    for (int j = 0; j < 3; j++)
    {
    }
  }

  esp_restart();
}



camera_fb_t *get_good_jpeg()
{

  camera_fb_t *fb;

  do
  {
    bp = millis();
    fb = esp_camera_fb_get();
    totalp = totalp + millis() - bp;

    int x = fb->len;
    int foundffd9 = 0;

    for (int j = 1; j <= 1025; j++)
    {
      if (fb->buf[x - j] != 0xD9)
      {
        // no d9, try next for
      }
      else
      {

        if (fb->buf[x - j - 1] == 0xFF)
        {

          if (j == 1)
          {
            normal_jpg++;
          }
          else
          {
            extend_jpg++;
          }
          if (j > 1000)
          { //  never happens. but > 1 does, usually 400-500
          }
          foundffd9 = 1;
          break;
        }
      }
    }

    if (!foundffd9)
    {
      bad_jpg++;
      esp_camera_fb_return(fb);
    }
    else
    {
      break;
      // count up the useless bytes
    }

  } while (1);

  return fb;
}

void start_avi()
{
  is_recording = true;
  sprintf(fname, "/root/video%d.avi",reset_counter);

  file_number++;

  avifile = fopen(fname, "w");
  idxfile = fopen("/root/idx.tmp", "w");

  if (avifile != NULL)
  {
  }
  else
  {

    major_fail();
    ESP_LOGE("START AVI", "BURAYA DUSTUM");
  }

  if (idxfile != NULL)
  {
  }
  else
  {

    major_fail();
  }

  for (i = 0; i < AVIOFFSET; i++)
  {
    char ch = pgm_read_byte(&avi_header[i]);
    buf[i] = ch;
  }

  size_t err = fwrite(buf, 1, AVIOFFSET, avifile);

  if (framesize == 6)
  {

    fseek(avifile, 0x40, SEEK_SET);
    err = fwrite(vga_w, 1, 2, avifile);
    fseek(avifile, 0xA8, SEEK_SET);
    err = fwrite(vga_w, 1, 2, avifile);
    fseek(avifile, 0x44, SEEK_SET);
    err = fwrite(vga_h, 1, 2, avifile);
    fseek(avifile, 0xAC, SEEK_SET);
    err = fwrite(vga_h, 1, 2, avifile);
  }
  else if (framesize == 10)
  {

    fseek(avifile, 0x40, SEEK_SET);
    err = fwrite(uxga_w, 1, 2, avifile);
    fseek(avifile, 0xA8, SEEK_SET);
    err = fwrite(uxga_w, 1, 2, avifile);
    fseek(avifile, 0x44, SEEK_SET);
    err = fwrite(uxga_h, 1, 2, avifile);
    fseek(avifile, 0xAC, SEEK_SET);
    err = fwrite(uxga_h, 1, 2, avifile);
  }
  else if (framesize == 9)
  {

    fseek(avifile, 0x40, SEEK_SET);
    err = fwrite(sxga_w, 1, 2, avifile);
    fseek(avifile, 0xA8, SEEK_SET);
    err = fwrite(sxga_w, 1, 2, avifile);
    fseek(avifile, 0x44, SEEK_SET);
    err = fwrite(sxga_h, 1, 2, avifile);
    fseek(avifile, 0xAC, SEEK_SET);
    err = fwrite(sxga_h, 1, 2, avifile);
  }
  else if (framesize == 7)
  {

    fseek(avifile, 0x40, SEEK_SET);
    err = fwrite(svga_w, 1, 2, avifile);
    fseek(avifile, 0xA8, SEEK_SET);
    err = fwrite(svga_w, 1, 2, avifile);
    fseek(avifile, 0x44, SEEK_SET);
    err = fwrite(svga_h, 1, 2, avifile);
    fseek(avifile, 0xAC, SEEK_SET);
    err = fwrite(svga_h, 1, 2, avifile);
  }
  else if (framesize == 5)
  {

    fseek(avifile, 0x40, SEEK_SET);
    err = fwrite(cif_w, 1, 2, avifile);
    fseek(avifile, 0xA8, SEEK_SET);
    err = fwrite(cif_w, 1, 2, avifile);
    fseek(avifile, 0x44, SEEK_SET);
    err = fwrite(cif_h, 1, 2, avifile);
    fseek(avifile, 0xAC, SEEK_SET);
    err = fwrite(cif_h, 1, 2, avifile);
  }

  fseek(avifile, AVIOFFSET, SEEK_SET);

  startms = millis();

  totalp = 0;
  totalw = 0;

  jpeg_size = 0;
  movi_size = 0;
  uVideoLen = 0;
  idx_offset = 4;

  frame_cnt = 0;

  bad_jpg = 0;
  extend_jpg = 0;
  normal_jpg = 0;

}

void another_save_avi(camera_fb_t *fb)
{

  int fblen;
  fblen = fb->len;

  jpeg_size = fblen;
  movi_size += jpeg_size;
  uVideoLen += jpeg_size;

  bw = millis();

  size_t dc_err = fwrite(dc_and_zero_buf, 1, 8, avifile);

  size_t err = fwrite(fb->buf, 1, fb->len, avifile);
  if (err != fb->len)
  {
  }

  remnant = (4 - (jpeg_size & 0x00000003)) & 0x00000003;

  print_2quartet(idx_offset, jpeg_size, idxfile);

  idx_offset = idx_offset + jpeg_size + remnant + 8;

  jpeg_size = jpeg_size + remnant;
  movi_size = movi_size + remnant;
  if (remnant > 0)
  {
    size_t rem_err = fwrite(zero_buf, 1, remnant, avifile);
  }

  fileposition = ftell(avifile);                          // Here, we are at end of chunk (after padding)
  fseek(avifile, fileposition - jpeg_size - 4, SEEK_SET); // Here we are the the 4-bytes blank placeholder

  print_quartet(jpeg_size, avifile); // Overwrite placeholder with actual frame size (without padding)

  fileposition = ftell(avifile);

  fseek(avifile, fileposition + jpeg_size, SEEK_SET);

  totalw = totalw + millis() - bw;

}

void end_avi()
{
  
  unsigned long current_end = 0;

  current_end = ftell(avifile);

  elapsedms = millis() - startms;

  float fRealFPS = (1000.0f * (float)frame_cnt) / ((float)elapsedms);

  float fmicroseconds_per_frame = 1000000.0f / fRealFPS;
  uint8_t iAttainedFPS = round(fRealFPS);
  uint32_t us_per_frame = round(fmicroseconds_per_frame);

  //Modify the MJPEG header from the beginning of the file, overwriting various placeholders

  fseek(avifile, 4, SEEK_SET);
  print_quartet(movi_size + 240 + 16 * frame_cnt + 8 * frame_cnt, avifile);

  fseek(avifile, 0x20, SEEK_SET);
  print_quartet(us_per_frame, avifile);

  unsigned long max_bytes_per_sec = movi_size * iAttainedFPS / frame_cnt;

  fseek(avifile, 0x24, SEEK_SET);
  print_quartet(max_bytes_per_sec, avifile);

  fseek(avifile, 0x30, SEEK_SET);
  print_quartet(frame_cnt, avifile);

  fseek(avifile, 0x8c, SEEK_SET);
  print_quartet(frame_cnt, avifile);

  fseek(avifile, 0x84, SEEK_SET);
  print_quartet((int)iAttainedFPS, avifile);

  fseek(avifile, 0xe8, SEEK_SET);
  print_quartet(movi_size + frame_cnt * 8 + 4, avifile);

  fseek(avifile, current_end, SEEK_SET);

  fclose(idxfile);

  size_t i1_err = fwrite(idx1_buf, 1, 4, avifile);

  print_quartet(frame_cnt * 16, avifile);

  idxfile = fopen("/root/idx.tmp", "r");


  char *AteBytes;
  AteBytes = (char *)malloc(8);

  for (int i = 0; i < frame_cnt; i++)
  {
    // size_t res = fread(AteBytes, 1, 8, idxfile);
    // size_t i1_err = fwrite(dc_buf, 1, 4, avifile);
    // size_t i2_err = fwrite(zero_buf, 1, 4, avifile);
    // size_t i3_err = fwrite(AteBytes, 1, 8, avifile);
  }

  free(AteBytes);
  fclose(idxfile);
  fclose(avifile);
  int xx = remove("/root/idx.tmp");
  isFinished = true;
}


void record_video()
{
  if (first)
  {

    first = 0;
  }

  frames++;
  frame_cnt = frames;

  if (frames == 1)
  { 
    we_are_already_stopped = 0;

    avi_start_time = millis();

    fb_curr = get_good_jpeg(); // should take zero time

    start_avi();

    fb_next = get_good_jpeg(); // should take nearly zero time due to time spent writing header

    another_save_avi(fb_curr);     // put first frame in avi
                                    // blink
    esp_camera_fb_return(fb_curr); // get rid of first frame

  }
  else if (millis() > (avi_start_time + avi_length * 1000) || !is_recording)
  { // end the avi

    fb_curr = fb_next;
    another_save_avi(fb_curr); // save final frame of avi
    esp_camera_fb_return(fb_curr);
    end_avi(); // end the movie
    // light off
    avi_end_time = millis();

    float fps = frames / ((avi_end_time - avi_start_time) / 100000);

    frames = 0; // start recording again on the next loop
    vTaskSuspend(video_record_t);
  }
  else
  {

    fb_curr = fb_next; // we will write a frame, and get the camera preparing a new one

    fb_next = get_good_jpeg(); // should take near zero, unless the sd is faster than the camera, when we will have to wait for the camera

    another_save_avi(fb_curr);

    esp_camera_fb_return(fb_curr);
    if (!is_recording)
    {
      end_avi();
    }
    
    
  }

}


