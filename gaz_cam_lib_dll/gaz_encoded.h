#ifndef GAZ_ENCODED_H
#define GAZ_ENCODED_H
#include <libavcodec/avcodec.h>

//typedef struct{
//    AVFrame *frame;
//    AVPacket *pkt;
//    AVCodecContext *c;
//}encoder_t;

//void init_encoder (encoder_t *);

//void encode(encoder_t *,FILE * );


typedef struct
{
    AVCodecContext *enc_ctx;
    AVFrame *frame;
    AVPacket *pkt;
}encoder_t;

encoder_t * init_encoder(int w,int h,int gop);
int ff_encoder(encoder_t *p_encoder,FILE * outline);
uint64_t GetMHClock(void);


#endif // GAZ_ENCODED_H
