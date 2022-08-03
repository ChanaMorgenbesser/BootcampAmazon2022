
#include <stdlib.h>
#include "function.h"
#include <stdio.h>
#include <dlfcn.h>
#include<assert.h>
#include <signal.h>
#include "entercommand.h"
#include "gaz_cam_lib_dll.h"
#define SIGINT 2
gas_api* p_gas;

handler_t * handler;
gas_api* load_library()
{
    void* libHandle= dlopen("/home/chana/PROJECTS/build-gaz_cam_lib_dll-Desktop-Debug/libgaz_cam_lib_dll.so.1.0.0",RTLD_LAZY);
    assert (libHandle);
    gas_api *p_gaz;
    p_gaz = (gas_api*)dlsym(libHandle,"gas_api_lib");
    return p_gaz;
}

void handle_sigint(int sig)
{
    if(sig==SIGINT)
   { p_gas->stop_record(handler);}
    //printf("Caught signal %d\n", sig);
}

int main()
{
   signal(SIGINT, handle_sigint);
    p_gas=load_library();
    handler= p_gas->init_dll();
  // p_gas->stop_record(handler);
   record_t r;
   snapshot_t t;
   p_gas->start_record(handler,r);
   //p_gas->do_snapshot(handler,t);

   mainCommadnd();
}

