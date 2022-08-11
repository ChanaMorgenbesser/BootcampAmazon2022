#include "gaz_cam_lib_dll.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include<pthread.h>
#include <unistd.h>
#include <queue.h>
#include <time.h>
#include <errno.h>


/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;
    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    return res;
}


int GAS_API_GET_DLL_VERSION(p_handler handler)
{
    return 1;
}

int GAS_API_START_RECORD(p_handler handler,record_t record)
{
    handler->status = handler->status|RECORD_ACTIVE;
    handler->status = handler->status&(~STOP_RECORD_ACTIVE);
    handler->record_stasus = 1;
    handler->status_snapshot = 0;
    pthread_t trd_counter;
    pthread_create(&handler->stg_capture->thread_id,NULL,capture,handler);
    pthread_create(&handler->stg_rgb_convertor->thread_id,NULL,rgb_convertor,handler);
    pthread_create(&handler->stg_yuv_convertor->thread_id,NULL,convert_yuv,handler);
    pthread_create(&handler->stg_decoder->thread_id,NULL,decoder,handler);
    //pthread_create(&handler->stg_write->thread_id,NULL,stg_write,handler);
    pthread_create(&trd_counter,NULL,check_trd_works,(handler->cnt));
    return 1;
}

void* check_trd_works(void* cnt1){
    counter * cnt = (counter*)cnt1;
    while(1){
        usleep(SLEEP_SECOND);
        printf("-------------------\n");
        printf("capture works %d time\n",cnt->counter_capture);
        cnt->counter_capture = 0;
        printf("rgb_convert works %d time\n",cnt->counter_rgb_converet);
        cnt->counter_rgb_converet = 0;
        printf("yov_convert works %d time\n",cnt->counter_yuv_convert);
        cnt->counter_yuv_convert = 0;
        printf("decoder works %d time\n",cnt->counter_decoder);
        cnt->counter_decoder = 0;
        printf("write works %d time\n",cnt->counter_write);
        cnt->counter_write = 0;
        printf("-------------------\n");

    }

}

int GAS_API_STOP_RECORD(p_handler handler)
{
    printf("GAS_API_STOP_RECORD try change\n");
    handler->record_stasus = 0;
    handler->status = handler->status&(~RECORD_ACTIVE);
    handler->status = handler->status|STOP_RECORD_ACTIVE;
    //1-valid
    //0-want stop
    //2-can exit
    while(handler->record_stasus!=2){
        msleep(10);
    }
    freeQueue(handler->stg_capture->src_queue);
    freeQueue(handler->stg_decoder->src_queue);
    freeQueue(handler->stg_rgb_convertor->src_queue);
    freeQueue(handler->stg_yuv_convertor->src_queue);
    exit(0);
    return 1;
}

int GAS_API_START_STREAMING(p_handler handler,streaming_t streaming)
{
    return 1;
}

int GAS_API_STOP_STREAMING(p_handler handler)
{
    return 1;
}

int GAS_API_DO_SNAPSHOT(p_handler handler,snapshot_t snapshot)
{
    handler->status_snapshot = 1;
    handler->status = handler->status|SNAPSHOT_ACTIVE;
    printf("GAS_API_DO_SNAPSHOT mkmkmkm\n");
    int *mat = (int *)malloc(ROW * COLUMN * sizeof(int));
    if(!mat){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    rgb_t * mat_rgb;

    if(handler->stg_capture->is_active == 1){

        while(isEmpty((handler)->stg_rgb_convertor->dest_queue)==1){
            msleep(10);
        }
        mat_rgb = ((rgb_t*)(top((handler)->stg_rgb_convertor->dest_queue)->data));
    }
    else{
        capture(handler);
        rgb_convertor(handler);

        while(isEmpty((handler)->stg_rgb_convertor->dest_queue) == 1){
            msleep(10);
        }
        mat_rgb = ((rgb_t*)(dequeue((handler)->stg_rgb_convertor->dest_queue))->data);
    }
    //    if(snapshot.type == bmp)
    //    {
    saveFrame_ppm(mat_rgb);
    //    }
    //    else{
    saveFrame_jpg((uint8_t *)mat_rgb,2);
    //}
    handler->status_snapshot = 0;
    handler->status = handler->status&(~SNAPSHOT_ACTIVE);
}

void saveFrame_ppm(rgb_t * mat_rgb){
    FILE * outfile = fopen("/home/chana/PROJECTS/Image/yeshua17.bmp","wb+");
    ppm_image * img = (ppm_image *)calloc(1,sizeof (ppm_image));
    img->width = ROW;
    img->height = COLUMN;
    img->data = (char *)mat_rgb;
    img->size = ROW*COLUMN*3;
    size_t n = 0;
    n += fprintf(outfile, "P6\n# THIS IS A COMMENT\n%d %d\n%d\n",
                 img->width, img->height, 0xFF);
    n += fwrite(img->data, 1, img->width * img->height * 3, outfile);
    fclose(outfile);
}

void saveFrame_jpg(uint8_t *pRGBBuffer, int iFrame)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    char szFilename[1024] = PATHSAVESNAPSHOT;
    int row_stride;
    FILE *fp;
    JSAMPROW row_pointer[1];//One row of bitmap
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    //sprintf(PATHSAVESNAPSHOT, "test%d.jpg", iFrame);
    sprintf(szFilename, "test%d.jpg", iFrame);//Picture name is video name + number
    fp = fopen("/home/chana/PROJECTS/Image/yeshua19.jpg", "wb");
    if(fp == NULL)
        return;

    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = ROW;//is the width and height of the image, in pixels
    cinfo.image_height = COLUMN;
    cinfo.input_components = 3;//Here is 1, which means grayscale image, if it is a color bitmap, it is 3
    cinfo.in_color_space = JCS_RGB;//JCS_GRAYSCALE means grayscale image, JCS_RGB means color image

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality (&cinfo, 80, 1);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = cinfo.image_width * 3;//The number of bytes in each row, if it is not an index image, it needs to be multiplied by 3 here

    //Compress each row
    while (cinfo.next_scanline <cinfo.image_height) {
        row_pointer[0] = &(pRGBBuffer[cinfo.next_scanline * row_stride]);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(fp);
}

int GAS_API_GET_VIDEO_STATISTIC(p_handler handler)
{
    return 1;
}

int GAS_API_GET_STATUS (p_handler handler)
{
    return handler->status;
}

handler_t * INIT_DLL(){

    handler_t * handler = (handler_t *)calloc(1,sizeof (handler_t));
    if(!handler){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    handler->status = 0;
    handler->record_stasus = 0;
    handler->stream_status = 0;
    handler->status_snapshot = 0;
    handler->cnt = (counter *)calloc(1,sizeof (counter));
    if(!handler->cnt){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    handler->stg_capture = (p_stage)calloc(1,sizeof(stage_t));
    if(!handler->stg_capture){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    handler->stg_capture->src_queue = NULL;
    handler->stg_capture->dest_queue = createQueue(10);
    handler->stg_rgb_convertor = (p_stage)calloc(1,sizeof(stage_t));
    if(!handler->stg_rgb_convertor){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    handler->stg_rgb_convertor->src_queue = handler->stg_capture->dest_queue;
    handler->stg_rgb_convertor->dest_queue = createQueue(10);
    handler->stg_yuv_convertor = (p_stage)calloc(1,sizeof (stage_t));
    if(!handler->stg_yuv_convertor){
        printf("the malloc unSucssed!!\n");
        exit(1);
    }
    handler->stg_yuv_convertor->src_queue = handler->stg_rgb_convertor->dest_queue;
    handler->stg_yuv_convertor->dest_queue = createQueue(10);
    handler->stg_decoder = (p_stage)calloc(1,sizeof (stage_t));
    handler->stg_decoder->src_queue = handler->stg_yuv_convertor->dest_queue;
    handler->stg_decoder->dest_queue = createQueue(10);
    handler->stg_write = (p_stage)calloc(1,sizeof (stage_t));
    handler->stg_write->src_queue = handler->stg_decoder->dest_queue;
    handler->stg_write->dest_queue = NULL;
    initRGB_static_mat(handler);
    return handler;
}

void initRGB_static_mat(p_handler handler)
{
    int i = 0,x = 0,y = 0,z = 255;
    for ( i = 0; i<20; i++) {
        handler->RGB_static_mat[0][i] = x;
        handler->RGB_static_mat[1][i] = y;
        handler->RGB_static_mat[2][i] = z;
        y += 10;
    }
    for ( i = 20; i < 40; i++) {
        handler->RGB_static_mat[0][i] = x;
        handler->RGB_static_mat[1][i] = y;
        handler->RGB_static_mat[2][i] = z;
        x += 12; y+= 2; z -= 8;
    }
    x = 255; y = 255; z = 95;
    for ( i = 40; i < 60; i++) {
        handler->RGB_static_mat[0][i] = x;
        handler->RGB_static_mat[1][i] = y;
        handler->RGB_static_mat[2][i] = z;
        y-=3; z-=5;
    }
    x = 255; y = 200; z = 0;
    for ( i = 60; i < 80; i++) {
        handler->RGB_static_mat[0][i] = x;
        handler->RGB_static_mat[1][i] = y;
        handler->RGB_static_mat[2][i] = z;
        y-=10;
    }
}

void* capture(void* handler){
    p_handler hand = (p_handler)handler;
    (hand)->record_stasus = 1;
    int j,k;
    int *mat;
    hand->stg_capture->is_active = 1;
    do{
        printf("-----capture-----\n");
        PRINTF_DBG("CAPTURE WORK\n");
        mat = (int *)malloc(ROW * COLUMN * sizeof(int));
        //malloc sucsses?
        if(!mat){
            printf("the malloc unSucssed!!\n");
            exit(1);
        }
        for (k = 0; k < ROW; k++ ) {
            for (j = 0; j < COLUMN; j++ ) {
                *(mat + k*COLUMN + j) = rand()%79 + 0;
            }
        }
        Node* node=createNode(mat);
        while(enqueue(hand->stg_capture->dest_queue,node) != 0){msleep(10);}
        hand->cnt->counter_capture++;
        msleep(2000);
    }//need change to status
    while(hand->status & RECORD_ACTIVE);
    hand->stg_capture->is_active = 0;
    return NULL;
}

void* rgb_convertor(void* handler){
    handler_t hand = *(p_handler)handler;
    (&hand)->stg_rgb_convertor->is_active = 1;
    int *base_mat;
    rgb_t * mat;
    do{
        PRINTF_DBG("RGB_CONVERTOR WORK\n");
        while(isEmpty((&hand)->stg_capture->dest_queue) == 1){
            PRINTF_DBG("///////////////////await!!\n");

            msleep(1);
        }
        printf("------rgb_convert----\n");
        Node * node=dequeue((&hand)->stg_capture->dest_queue);
        base_mat = (int*)((node)->data);

        mat = (rgb_t *)malloc(ROW * COLUMN * sizeof(rgb_t));
        if(!mat){
            printf("the malloc unSucssed!!\n");
            exit(1);
        }
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COLUMN; j++){
                PRINTF_DBG("temp_mat[%d][%d]  %d\n",i,j,*(base_mat + i*320 + j));
            }
        }
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COLUMN; j++){
//                mat[i*COLUMN+j].r = 255;
//                mat[i*COLUMN+j].g = 0;
//                mat[i*COLUMN+j].b = 0;
                mat[i*COLUMN+j].r = (&hand)->RGB_static_mat[0][*(base_mat + i*COLUMN + j)];
                mat[i*COLUMN+j].g = (&hand)->RGB_static_mat[1][*(base_mat + i*COLUMN + j)];
                mat[i*COLUMN+j].b = (&hand)->RGB_static_mat[2][*(base_mat + i*COLUMN + j)];
            }
        }
        Node * n=createNode(mat);
        while(enqueue((&hand)->stg_rgb_convertor->dest_queue,n)!=0);
        free(base_mat);
        free(node);
        hand.cnt->counter_rgb_converet++;
    }
    while(((&hand)->status&RECORD_ACTIVE|| isEmpty((&hand)->stg_capture->dest_queue) == 0));
    (&hand)->stg_rgb_convertor->is_active=0;
    return NULL;
}

void* convert_yuv(void* handler){
    handler_t hand = *(p_handler)handler;
    rgb_t * rgb_mat;
    YUV *yuv;
    while((&hand)->status&RECORD_ACTIVE || isEmpty((&hand)->stg_rgb_convertor->dest_queue) == 0)
    {
        while(isEmpty((&hand)->stg_rgb_convertor->dest_queue) == 1){
            PRINTF_DBG("await in convert yov!!\n");
        }
        printf("------yuv------\n");
        yuv = (YUV*)calloc(1,sizeof(YUV));
        //calloc sucsses?
        if(!yuv){

            printf("the malloc unSucssed!!\n");
            exit(1);
        }
        while((&hand)->status_snapshot == 1){
            msleep(10);
        }
        Node * n=dequeue((&hand)->stg_rgb_convertor->dest_queue);

        rgb_mat = ((rgb_t*)((n)->data));
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COLUMN; j++){
                PRINTF_DBG("------------------------------------------CONVERT_YUV IN LOOP------------------------------------------\n");
                unsigned char R = (char)rgb_mat[i*COLUMN+j].r;
                unsigned char G = (char)rgb_mat[i*COLUMN+j].g;
                unsigned char B = (char)rgb_mat[i*COLUMN+j].b;
                yuv->y[COLUMN*i+j] = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
                if(i%2 == 0 && j%2 == 0){
                    yuv->u[(COLUMN*i+j)/4] =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
                    yuv->v[(COLUMN*i+j)/4] = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;

                }

            }
        }
        while((&hand)->status_snapshot==1);
        free(rgb_mat);
        free(n);
        hand.cnt->counter_yuv_convert++;
        msleep(500);
        Node * node=createNode(yuv);
        while(enqueue((&hand)->stg_yuv_convertor->dest_queue,node)!=0){
            msleep(1);
        }

    }
    (&hand)->record_stasus=2;
    return NULL;
}



void* decoder(void* handler){

    handler_t hand=*(p_handler)handler;
    int ret=0,count=0;
    AVFrame *frame=NULL;
    FILE *outline=NULL;
    encoder_t *p_encoder;
    uint64_t start_time=0;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    fflush(stdout);
    p_encoder=init_encoder(COLUMN,ROW,12);
    frame=p_encoder->frame;

    outline=fopen("/home/chana/PROJECTS/Videoes/michal.ts","wb");
    assert(outline);

    while((&hand)->status&RECORD_ACTIVE || isEmpty((&hand)->stg_yuv_convertor->dest_queue) == 0)
    {
        YUV * yuv;
        while(isEmpty((&hand)->stg_yuv_convertor->dest_queue) == 1){
            PRINTF_DBG("DECODERRRRRRRRRRRRRRRRRR    await!!\n");
        }
        printf("------decoder------\n");
        PRINTF_DBG("------------------------------------------DECODER--------------------------------------------\n");

        Node * node=dequeue((&hand)->stg_yuv_convertor->dest_queue);
        yuv = ((YUV*)((node)->data));

        ret = av_frame_make_writable(frame);
        assert(ret >=0);

        for (int i=0;i<frame->height;i++)
             for (int j=0;j<frame->width;j++)
                    frame->data[0][i*frame->linesize[0]+j]=yuv->y[i*frame->width+j/2];
         for(int i=0;i<p_encoder->enc_ctx->height/2;i++)
                   for(int j=0;j<p_encoder->enc_ctx->width/2;j++){
                      frame->data [1][i*frame->linesize[1]+j]=yuv->u[(i/2)*frame->width+j/2];
                      frame->data [2][i*frame->linesize[2]+j]= yuv->v[(i/2)*frame->width+j/2];
              }

        if (start_time==0){
            frame->pts=0;
            start_time=GetMHClock();
        }
        else
            frame->pts = GetMHClock()-start_time;

        ff_encoder(p_encoder,outline);
        hand.cnt->counter_decoder++;
    }
    fwrite(endcode, 1, sizeof(endcode), outline);
    fclose(outline);
    return NULL;
}



void* stg_write(void* handler){
    handler_t hand=*(p_handler)handler;
    while(1)
    {
        while(isEmpty((&hand)->stg_decoder->dest_queue)==1){
            PRINTF_DBG("WRITEEEEEEEEEEEEEEEEEEEEEEEEE   await!!\n");
        }
        PRINTF_DBG("--------------------------------------WRITE--------------------------------------------\n");
    }
    return NULL;
}

gas_api gas_api_lib={
    .get_dll_version = GAS_API_GET_DLL_VERSION,
    .start_record = GAS_API_START_RECORD,
    .stop_record = GAS_API_STOP_RECORD,
    .start_streaming = GAS_API_START_STREAMING,
    .stop_streaming = GAS_API_STOP_STREAMING,
    .do_snapshot = GAS_API_DO_SNAPSHOT,
    .get_video_statistic = GAS_API_GET_VIDEO_STATISTIC,
    .get_status = GAS_API_GET_STATUS,
    .init_dll = INIT_DLL,
    .msleep = msleep
};







