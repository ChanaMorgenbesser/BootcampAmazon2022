

#include <QCoreApplication>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "function.h"
#include <stdio.h>
#include <dlfcn.h>
#include<assert.h>
#include "gaz_cam_lib_dll.h"


gas_api* load_library()
{
    void* libHandle= dlopen("/home/chana/PROJECTS/build-gaz_cam_lib_dll-Desktop-Debug/libgaz_cam_lib_dll.so.1.0.0",RTLD_LAZY);
    assert (libHandle);
    gas_api *p_gaz;
    p_gaz = (gas_api*)dlsym(libHandle,"gas_api_lib");
    return p_gaz;
}
int main()
{
   gas_api* p_gas=load_library();
   handler_t * handler= p_gas->init_dll();
    p_gas->stop_record(handler);
    record_t r{};
    snapshot_t t{};
    //p_gas->start_record(handler,r);
    p_gas->do_snapshot(handler,t);
   Func* func= reset();
   while(1){
       printf("enter a command\n");
       char command[100];
       int i=0;
       fgets(command,100,stdin);
       command[strlen(command)-1]='\0';
       char* token;
       int num=strlen(command);
       token=strtok(command," ");
       int flag=0;
       if(strcmp("help",token)==0){
           flag=1;
           token=strtok(NULL," ");}
       Func* temp=func;
       while(temp!=NULL&&strcmp(temp->name_function,token)!=0)
           temp=temp->next;

   if(temp!=NULL){
       if(flag==1)
           printf("%s",temp->f_help);
       else
           temp->command(command+strlen(token)+1);
   }

   else
   printf("command not found\n");

   }
}
