//#include "snap_shot.h"


//int GAS_API_DO_SNAPSHOT(p_handler handler,snapshot_t snapshot)
//{

//    printf("GAS_API_DO_SNAPSHOT\n");
//    //lock samafor
//   // if(handler->stg_capture->is_active==1){

//    handler->status_snapshot=1;
//        capture(handler);
//        handler->status_snapshot=1;
//        rgb_convertor(handler);
//        FILE * outfile=fopen("/home/chana/PROJECTS/Image/t.bmp","wb+");
//        ppm_image * img;
//        img->width=240;
//        img->height=320;
//        img->data=(char *)handler->value_snapshot;
//        img->size=240*320*3;
//        size_t n = 0;
//         n += fprintf(outfile, "P6\n# THIS IS A COMMENT\n%d %d\n%d\n",
//                      img->width, img->height, 0xFF);
//         n += fwrite(img->data, 1, img->width * img->height * 3, outfile);
//         fclose(outfile);
//         return n;
//   // }
//    //unlock samafor
//    return 1;
//}
