#ifndef GPIO_H
#define GPIO_H

enum STATE{ON,OFF,CONST_ON};
typedef struct flash_t{
    int * rate;
    int num_gpio;
    enum STATE state;
}flash_t;
void reset_flash(flash_t*, int,int,enum STATE);
void on_led(int);
void off_led(int);
void * flash(void *);
void* main_GPIO(void *);
static int active;
static int rate;

#endif // GPIO_H
