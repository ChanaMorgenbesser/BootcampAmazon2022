#include "gpio.h"
#include "gaz_cam_lib_dll.h"
extern gas_api* p_gas;
extern p_handler handler;

void on_led(int num_gpio){


}
void off_led(int num_gpio){

}
void * flash(void * my_flash){
    flash_t * flsh=(flash_t *)my_flash;
    while(*(flsh->rate)>=0){
        switch(flsh->state){
            case ON:
                on_led(flsh->num_gpio);
                flsh->state=OFF;
                break;
            case OFF:
                off_led(flsh->num_gpio);
                flsh->state=ON;
                break;
           case CONST_ON:
                on_led(flsh->num_gpio);
                break;
        }
        p_gas->msleep(*(flsh->rate));
    }
    off_led(flsh->num_gpio);
    return NULL;
}

void reset_flash(flash_t * f,int rate,int num_gpio,enum STATE state){
    f->num_gpio=num_gpio;
    f->rate=&rate;
    f->state=state;
}

void main_GPIO(){

    flash_t * flash_record=(flash_t*)calloc(1,sizeof (flash_t));
    if(!flash_record){
        printf("ERORR\n");
        return;
    }
    flash_t * flash_stop=(flash_t*)calloc(1,sizeof (flash_t));
    if(!flash_stop){
        printf("ERORR\n");
        return;
    }
    active=1;
    int status,prev_status=0;
    pthread_t trd_record,trd_stop_record;
    while(active){
        status=p_gas->get_status(handler);
        if(status!=prev_status){
            if(status & RECORD_ACTIVE){
                reset_flash(flash_record,500,24,ON);
                pthread_create(trd_record,NULL,flash,flash_record);
            }
            else{
                *(flash_record->rate)=-1;
            }
            if(status & STOP_RECORD_ACTIVE){
                reset_flash(flash_stop,200,26,ON);
                pthread_create(trd_stop_record,NULL,flash,flash_stop);
            }
            else{
                *(flash_stop->rate)=-1;
            }
        }
        prev_status=status;
        p_gas->msleep(500);
    }
    free(flash_record);
    free(flash_stop);

}

