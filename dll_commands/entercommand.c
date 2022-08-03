#include "entercommand.h"
#include <stdio.h>

void mainCommadnd(){
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
