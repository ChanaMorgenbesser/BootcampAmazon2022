#ifndef GAS_LIB_H
#define GAS_LIB_H
#define ROW 240
#define COLUMN 320
#include <pthread.h>
#include <stdio.h>
//#include<iostream>

#include "queue.h"
//#include "snap_shot.h"



 typedef struct streaming_t{
     int ip;

     int port;

 }streaming_t;
//-----------------
 typedef struct snapshot{
     char* file_name;
     int width;
     int height;
     int type;
 }snapshot_t;

typedef struct {
    int width;
    int height;
    char *data;
    size_t size;
} ppm_image;

typedef struct{
   unsigned  char r;
   unsigned char g;
   unsigned char b;
}rgb_t;


typedef struct{
    char y[240*320];
    char u[240*320/4];
    char v[240*320/4];
}YUV;

enum status_stage{
   CAPTURE_ACTIVE      = 1,
   RECORD_ACTIVE       = 2,
   STOP_RECORD_ACTIVE  = 4,
   SNAPSHOT_ACTIVE     = 8
};



void rgb_snap(int * mat_capture,rgb_t * rgb_mat);
int * capture_snap();

//---------------
 typedef struct gpio_t{
     int num;
     int dir;
     int value;
     int is_blink;
     int opertion;
 }gpio_t;

 typedef struct record_t{
     char* file_name;
     int codec;
     int width;
     int height;
     int fps;

}record_t;


typedef struct stage
{
    pthread_t thread_id;
    int is_active;
    QueueC* src_queue;
    QueueC* dest_queue;
}stage_t,*p_stage;


typedef struct handler
{
    int status;
    int record_stasus;
    int stream_status;
    int RGB_static_mat[3][80];
    p_stage stg_capture;
    p_stage stg_rgb_convertor;
    p_stage stg_yuv_convertor;
    p_stage stg_decoder;
    p_stage stg_write;
    void * value_snapshot;
    int status_snapshot;
}handler_t,*p_handler;



 typedef struct {
  p_handler(*init_dll)();
  int (*start_record)(p_handler,record_t);
  int (*stop_record)(p_handler);
  int (*start_streaming)(p_handler,streaming_t);
  int (*stop_streaming)(p_handler);
  int (*do_snapshot)(p_handler,snapshot_t);
  int (*get_status)(p_handler);
  int (*get_video_statistic)(p_handler);
  int (*get_dll_version)(p_handler);
  int (*msleep)(long);
 }gas_api;

handler_t * INIT_DLL();
int msleep(long msec);
void* capture(void*);
void* rgb_convertor(void*);
void* convert_yuv(void*);
void* decoder(void*);
void* stg_write(void*);
int GAS_API_GET_DLL_VERSION(p_handler);
int GAS_API_START_RECORD(p_handler,record_t);
int GAS_API_STOP_RECORD(p_handler);
int GAS_API_START_STREAMING(p_handler,streaming_t);
int GAS_API_DO_SNAPSHOT(p_handler,snapshot_t);
int GAS_API_STOP_STREAMING(p_handler);
int GAS_API_GET_VIDEO_STATISTIC(p_handler);
int GAS_API_GET_STATUS (p_handler);
void initRGB_static_mat(p_handler );
#endif // GAS_LIB_H
