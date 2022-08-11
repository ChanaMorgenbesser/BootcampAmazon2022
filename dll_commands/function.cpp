#include "function.h"
#include <stdlib.h>
#include <string>
#include <QCoreApplication>
#include <iostream>
void start_record(char* param){
    char** arr=splitArg(param,4);
    printf("arr0%s\n",arr[0]);
    int valid=0;
    if(arr[0]){
        if(validParam("int",arr[0])==0){
            printf("param not valid");
            return;
        }
}
    else{arr[0]="0";}
    if(arr[1]){
        if(validParam("string",arr[1])==0){

            printf("string param not valid\n");
           // return;
        }

        arr[1]=arr[1]+1;
        arr[1]=strtok(arr[1],"\"");

}
      else{arr[1]="null";}
    if(arr[2]){
        if(validParam("double",arr[2])==0){
            printf("double param not valid\n");
            return;
        }
}
      else{arr[2]="0.0";}
    if(arr[3]){
        if(validParam("int",arr[3])==0){
            printf("int param not valid\n");
            return;
        }
}
      else{arr[3]="0";}



    start_record(atoi(arr[0]),arr[1],atof(arr[2]),atoi(arr[3]));
}
void start_record(int n1, char* n2,double n3,int n4){
             printf("num1 %d num2 %s num3 %f num4 %d\n",n1,n2,n3,n4);

             }
void stop_record(char*param){
     printf("stop\n");
    char** arr=splitArg(param,4);
}
void get_status(char* param){
    printf("status\n");
    char** arr=splitArg(param,4);
}
char** splitArg(char* param,int num){
   // char* arr[num];//arr of prameters
    char** arr=(char**)calloc(num,sizeof(char*));
    char delim[2]=" ";//arr of delim
    char* temp;
    if(strcmp(param," ")==0){
       printf("error\n");
       return arr;
    }
      temp=strtok(param,delim);
      arr[0]=temp;
      for  (int i=1;i<num; i++){
          temp=strtok(NULL,delim);
          if(temp)
          {
          arr[i]=temp;  }
          else{
              printf("no parameter\n");
              arr[i]=NULL;
              break;}   }
     for(int i=0;i<4;i++){
        printf("arr%d= %s\n",i,arr[i]);
    }
return arr;
}
Func* reset(){
    Func* func=NULL;
    func=registerFunc(&func,"sr",start_record,"help start record");
        func=registerFunc(&func,"stop",stop_record,"help stop record");
        func=registerFunc(&func,"status",get_status,"help status");
return func;
}
Func*  registerFunc(Func** pfunc, char* name,void(*command)(char*),char* help){
    Func* temp= (Func*)calloc(1,sizeof (Func));
    temp->command=command;
    temp->f_help=help;
    temp->name_function=name;
    temp->next=*pfunc;
    *pfunc=temp;
    return *pfunc;
}
int validParam(char * type,char* param){
    int len=strlen(param);
    if(strcmp(type,"int")==0)
    {
        for(int i=0;i<len;i++){
            if((strcmp(param+i,"0")!=0)&&(atoi(param+i)==0)){
                return 0;
            }
        }
        return 1;
    }
    if(strcmp(type,"string")==0)
    {

  if(strcmp(param+len-1,"\"")==0){
      char c[2]={};
      c[0]=*param;
      c[1]='\0';
        if(strcmp(c,"\"")==0)
          {
            return 1;
        }}
        return 0;
    }
    if(strcmp(type,"double")==0)
    {
        for(int i=0;i<len;i++){
            if((strcmp(param+i,"0")!=0)&&(atof(param+i)==0)){
                return 0;
            }
        }
        return 1;
    }
    return 0;
}
