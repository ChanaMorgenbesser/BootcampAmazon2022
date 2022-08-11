#include "gaz_encoded.h"
#include <assert.h>


encoder_t * init_encoder(int w,int h,int gop){
    const AVCodec *codec;
    int ret=0;
    encoder_t *p_encoder =(encoder_t*) calloc (1,sizeof (encoder_t)) ;
    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
    assert(codec);
    p_encoder->frame=av_frame_alloc();

    assert(p_encoder->frame);

    p_encoder->frame->width=w;
    p_encoder->frame->height=h;
    p_encoder->frame->format=AV_PIX_FMT_YUV420P;
    ret = av_frame_get_buffer(p_encoder->frame, 0);
    assert(ret>=0);

    p_encoder->enc_ctx=avcodec_alloc_context3(codec);
   assert(p_encoder->enc_ctx) ;

   p_encoder->pkt=av_packet_alloc();
   assert(p_encoder->pkt);
   p_encoder->enc_ctx->bit_rate=400000;
   p_encoder->enc_ctx->width=w;
   p_encoder->enc_ctx->height=h;

   p_encoder->enc_ctx->time_base = (AVRational){1, 25};
   p_encoder->enc_ctx->framerate = (AVRational){25, 1};
   p_encoder->enc_ctx->gop_size = gop;
   p_encoder->enc_ctx->max_b_frames = 1;
   p_encoder->enc_ctx->pix_fmt=AV_PIX_FMT_YUV420P;

   ret=avcodec_open2(p_encoder->enc_ctx, codec, NULL);
   assert(ret>=0);
   return (p_encoder);

}

int ff_encoder(encoder_t *p_encoder,FILE * outfile){
     int ret=0;
     ret = avcodec_send_frame(p_encoder->enc_ctx, p_encoder->frame);
     assert(ret >= 0);
     while (ret >= 0) {
         ret = avcodec_receive_packet(p_encoder->enc_ctx, p_encoder->pkt);

         if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
             return 0;
         assert(ret >= 0) ;
         fwrite(p_encoder->pkt->data, 1, p_encoder->pkt->size, outfile);
         av_packet_unref(p_encoder->pkt);


    }

}


uint64_t GetMHClock(void){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)tv.tv_sec * 1000000L + (uint64_t)tv.tv_usec;
}























//void init_encoder (encoder_t * encoder){
//    const char *codec_name;
//    const AVCodec *codec;
////    int i, ret, x, y;
////    FILE *f;
//    int ret;
//    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
//    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
//    if (!codec) {
//        fprintf(stderr, "Codec '%s' not found\n", codec_name);
//        exit(1);
//    }
//    encoder->c = avcodec_alloc_context3(codec);
//    if (!encoder->c) {
//        fprintf(stderr, "Could not allocate video codec context\n");

//        exit(1);
//    }
//    encoder->pkt = av_packet_alloc();
//    if (!encoder->pkt)
//        exit(1);
//    /* put sample parameters */
//    encoder->c->bit_rate = 400000;
//    /* resolution must be a multiple of two */
//    encoder->c->width = 240;
//    encoder->c->height = 320;
//    /* frames per second */
//    encoder->c->time_base = (AVRational){1, 25};
//    encoder->c->framerate = (AVRational){25, 1};
//    encoder->c->gop_size = 20;
//    encoder->c->max_b_frames = 1;
//    encoder->c->pix_fmt=AV_PIX_FMT_YUV420P;
//    ret = avcodec_open2(encoder->c, codec, NULL);
////    if (ret < 0) {
////        //fprintf(stderr, "Could not open codec: %s\n", av_err2str(ret));
////        exit(1);
////    }
//    encoder->frame = av_frame_alloc();
//    if (!encoder->frame) {
//        fprintf(stderr, "Could not allocate video frame\n");
//        exit(1);
//    }
//    encoder->frame->format = encoder->c->pix_fmt;
//    printf("=====%d=====\n",encoder->frame->format);
//    encoder->frame->width  = encoder->c->width;
//    encoder->frame->height = encoder->c->height;
//    ret = av_frame_get_buffer(encoder->frame,0);
//    assert(ret>=0);
//}

//int count=0;

//void encode(encoder_t * encoder,FILE * outfile)
//{
//    int ret;
//    ret = avcodec_send_frame(encoder->c, encoder->frame);
//    if (ret < 0) {
//        fprintf(stderr, "Error sending a frame for encoding\n");
//        exit(1);
//   }
//    while (ret >= 0) {
//        ret = avcodec_receive_packet(encoder->c, encoder->pkt);
//        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
////        {
////            fwrite(encoder->frame->data, 1, sizeof(encoder->frame->data), outfile);
////            av_packet_unref(encoder->pkt);
////        }
//            return;
//        else if (ret < 0) {
//            fprintf(stderr, "Error during encoding\n");
//            exit(1);
//        }
//        fwrite(encoder->pkt->data, 1, encoder->pkt->size, outfile);
//        av_packet_unref(encoder->pkt);
//    }
//}


