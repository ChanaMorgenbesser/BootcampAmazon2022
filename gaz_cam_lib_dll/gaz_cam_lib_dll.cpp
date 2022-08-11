#include "gaz_cam_lib_dll.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<pthread.h>
#include <unistd.h>

int GAS_API_GET_DLL_VERSION(p_handler handler)
{
    return 1;

}


int GAS_API_START_RECORD(p_handler handler,record_t record)
{
    pthread_create(&handler->stg_capture->thread_id,NULL,capture,handler);
    pthread_create(&handler->stg_rgb_convertor->thread_id,NULL,rgb_convertor,handler);
    pthread_create(&handler->stg_yuv_convertor->thread_id,NULL,convert_yuv,handler);
    pthread_create(&handler->stg_decoder->thread_id,NULL,decoder,handler);
    pthread_create(&handler->stg_write->thread_id,NULL,stg_write,handler);
    pthread_join(handler->stg_capture->thread_id,NULL);
    pthread_join(handler->stg_rgb_convertor->thread_id,NULL);
    pthread_join(handler->stg_yuv_convertor->thread_id,NULL);
    pthread_join(handler->stg_decoder->thread_id,NULL);

    pthread_join(handler->stg_write->thread_id,NULL);
    return 1;

}

int GAS_API_STOP_RECORD(p_handler handler)
{
    printf("GAS_API_STOP_RECORD\n");
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


    printf("GAS_API_DO_SNAPSHOT mkmkmkm\n");
    int *mat = (int *)malloc(ROW * COLUMN * sizeof(int));
    int k,j;
    //malloc sucsses?
    for (k=0;k<ROW ;k++ ) {
        for (j=0;j<COLUMN ;j++ ) {
            *(mat + k*COLUMN + j)=rand()%79+0;
        }
    }
    rgb_t mat_rgb[ROW*COLUMN];

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLUMN;j++){
            mat_rgb[i*COLUMN+j].r=(handler)->RGB_static_mat[0][*(mat + i*COLUMN + j)];
            mat_rgb[i*COLUMN+j].g=(handler)->RGB_static_mat[1][*(mat + i*COLUMN + j)];
            mat_rgb[i*COLUMN+j].b=(handler)->RGB_static_mat[2][*(mat + i*COLUMN + j)];
           // mat_rgb[i*COLUMN+j].b=0;
        }

    }

    FILE * outfile=fopen("/home/chana/PROJECTS/Image/try6.bmp","wb+");
    ppm_image * img=(ppm_image *)calloc(1,sizeof (ppm_image));
    img->width=ROW;
    img->height=COLUMN;


    img->data=(char *)mat_rgb;
    img->size=ROW*COLUMN*3;
    size_t n = 0;
    n += fprintf(outfile, "P6\n# THIS IS A COMMENT\n%d %d\n%d\n",
                 img->width, img->height, 0xFF);
    n += fwrite(img->data, 1, img->width * img->height * 3, outfile);
    fclose(outfile);


    return n;

}


int GAS_API_GET_VIDEO_STATISTIC(p_handler handler)
{
    return 1;
}

int GAS_API_GET_STATUS (p_handler handler)
{
    return 1;
}
handler_t * INIT_DLL(){
    handler_t * handler=(handler_t *)calloc(1,sizeof (handler_t));

    handler->value_snapshot=NULL;
    handler->status_snapshot=0;

    handler->stg_capture=(p_stage)calloc(1,sizeof(stage_t));
    handler->stg_capture->src_queue=NULL;
    handler->stg_capture->dest_queue=createQueue(10);

    handler->stg_rgb_convertor=(p_stage)calloc(1,sizeof(stage_t));
    handler->stg_rgb_convertor->src_queue=handler->stg_capture->dest_queue;
    handler->stg_rgb_convertor->dest_queue=createQueue(10);


    handler->stg_yuv_convertor=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_yuv_convertor->src_queue=handler->stg_rgb_convertor->dest_queue;
    handler->stg_yuv_convertor->dest_queue=createQueue(10);

    handler->stg_decoder=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_decoder->src_queue=handler->stg_yuv_convertor->dest_queue;
    handler->stg_decoder->dest_queue=createQueue(10);

    handler->stg_write=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_write->src_queue=handler->stg_decoder->dest_queue;
    handler->stg_write->dest_queue=NULL;
    initRGB_static_mat(handler);
    return handler;
}
void initRGB_static_mat(p_handler handler)
{
    int i=0,x=0,y=0,z=255;
    for ( i=0;i<20 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y+=10;
    }
    for ( i=20;i<40 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        x+=12;y+=2;z-=8;
    }
    x=255;y=255;z=95;
    for ( i=40;i<60 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y-=3;z-=5;
    }
    x=255;y=200;z=0;
    for ( i=60;i<80 ; i++) {
        handler->RGB_static_mat[0][i]=x;

        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y-=10;

    }
}






void* capture(void* handler){

    p_handler hand=(p_handler)handler;
    int j,k;
    int *mat;
    hand->stg_capture->is_active=1;
    while(1){
        mat = (int *)malloc(ROW * COLUMN * sizeof(int));
        //malloc sucsses?
        for (k=0;k<ROW ;k++ ) {
            for (j=0;j<COLUMN ;j++ ) {
                *(mat + k*COLUMN + j)=rand()%79+0;

            }
        }   

        for (k=0;k<ROW ;k++ ) {
            for (j=0;j<COLUMN ;j++ ) {
                printf("mat[%d][%d] = %d \n",k,j,*(mat + k*COLUMN + j));
            }


        }

        printf("  hand->stg_capture->dest_queue    %s\n",hand->stg_capture->dest_queue);
        while(enqueue(hand->stg_capture->dest_queue,createNode(&mat))!=0);


        printf("%d\n",1);
        sleep(0.4);
    }
    hand->stg_capture->is_active=0;
}

void* rgb_convertor(void* handler){
    printf("-------------------------------------------------------------------------------------------\n");
    handler_t hand=*(p_handler)handler;
    (&hand)->stg_rgb_convertor->is_active=1;

    printf("hand->stg_rgb_convertor->is_active=1 : %d \n",(&hand)->stg_rgb_convertor->is_active);
    int *base_mat ;
    rgb_t * mat;
    while(1){
        printf("  hand->stg_capture->dest_queue     %s\n",(&hand)->stg_capture->dest_queue);
        base_mat = (int *)malloc(ROW * COLUMN * sizeof(int));
        while(isEmpty((&hand)->stg_capture->dest_queue)==1){
            printf("///////////////////await!!\n");
        }
        base_mat=*((int**)(dequeue((&hand)->stg_capture->dest_queue)->data));
        mat = (rgb_t *)malloc(ROW * COLUMN * sizeof(rgb_t));
        for(int i=0;i<ROW;i++){
            for(int j=0;j<COLUMN;j++){
                printf("       temp_mat[%d][%d]        gfhbjnkm                %d\n",i,j,*(base_mat + i*320 + j));
            }
        }
        for(int i=0;i<ROW;i++){
            for(int j=0;j<320;j++){
                mat[i*COLUMN+j].r=(&hand)->RGB_static_mat[0][*(base_mat + i*COLUMN + j)];
                mat[i*COLUMN+j].g=(&hand)->RGB_static_mat[1][*(base_mat + i*COLUMN + j)];
                mat[i*COLUMN+j].b=(&hand)->RGB_static_mat[2][*(base_mat + i*COLUMN + j)];
            }
        }

       while(enqueue((&hand)->stg_rgb_convertor->dest_queue,createNode(&mat))!=0);
    }
    free(base_mat);
    (&hand)->stg_rgb_convertor->is_active=0;
}

void* convert_yuv(void* handler){

    handler_t hand=*(p_handler)handler;
    rgb_t * rgb_mat;
    YUV *yuv;
    while(1)
    {
        while(isEmpty((&hand)->stg_rgb_convertor->dest_queue)==1){
            printf("***************************************888await!!\n");
        }
        yuv=(YUV*)calloc(1,sizeof(YUV));



//  if(!mat){
//         printf("the malloc unSucssed!!\n");
//         exit(1);
//     }


        printf("------------------------------------------CONVERT_YUV-\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\-----------------\n");
//        rgb_mat = (rgb_t *)malloc(ROW * COLUMN * sizeof(rgb_t));
//        rgb_mat=*((rgb_t**)(dequeue((&hand)->stg_rgb_convertor->dest_queue)->data));
//        for(int i=0;i<ROW;i++){
//          for(int j=0;j<COLUMN;j++){
//                unsigned char R=(int)rgb_mat[i*COLUMN+j].r;
//                unsigned char G=(int)rgb_mat[i*COLUMN+j].g;
//                unsigned char B=(int)rgb_mat[i*COLUMN+j].b;
//                yuv->y[COLUMN*i+j]=(0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
//                if(i%2==0&&j%2==0){
//                    yuv->u[(COLUMN*i+j)/4] =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
//                    yuv->v[(COLUMN*i+j)/4]  = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
//                }
//            }

//        }



    }
    return NULL;


}
void* decoder(void* handler){
    // p_handler hand=(p_handler)handler;
    handler_t hand=*(p_handler)handler;
    while(1)
    {
        while(isEmpty((&hand)->stg_yuv_convertor->dest_queue)==1){
            printf("DECODERRRRRRRRRRRRRRRRRR    await!!\n");
        }
        printf("------------------------------------------DECODER--------------------------------------------\n");
    }
    return NULL;
}
void* stg_write(void* handler){
    // p_handler hand=(p_handler)handler;
    handler_t hand=*(p_handler)handler;
    while(1)
    {
        while(isEmpty((&hand)->stg_decoder->dest_queue)==1){
            printf("WRITEEEEEEEEEEEEEEEEEEEEEEEEE   await!!\n");
        }
        printf("------------------------------------------WRITE--------------------------------------------\n");
    }
    return NULL;
}


gas_api gas_api_lib={
    .get_dll_version=GAS_API_GET_DLL_VERSION,
    .start_record=GAS_API_START_RECORD,
    .stop_record=GAS_API_STOP_RECORD,
    .start_streaming=GAS_API_START_STREAMING,
    .stop_streaming=GAS_API_STOP_STREAMING,
    .do_snapshot=GAS_API_DO_SNAPSHOT,
    .get_video_statistic=GAS_API_GET_VIDEO_STATISTIC,
    .get_status=GAS_API_GET_STATUS,
    .init_dll=INIT_DLL
};

