#include "jsondata.h"


void initMyJson(){
    myJson=(MyJson*)calloc(1,sizeof (MyJson));
    char * p="/home/chana/Documents/yeshuaAMENNN";
    myJson->fullPath=(char*)calloc(strlen(p),sizeof (char));
    strcpy(myJson->fullPath,p);
    myJson->snapshotData=(snapshot_tt *)calloc(1,sizeof (snapshot_tt));
    myJson->recordDeta=(record_tt *)calloc(1,sizeof (record_tt));
    myJson->recordDefultData=(record_tt *)calloc(1,sizeof (record_tt));

    myJson->buffer=(char*)calloc(800,sizeof(char));
    myJson->fp=NULL;
    myJson->snapshotDataDEFULT=(snapshot_tt *)calloc(1,sizeof (snapshot_tt));
    myJson->snapshotData->privateBuffer=(char *)calloc(100,sizeof (char));
    myJson->snapshotDataDEFULT->privateBuffer=(char *)calloc(100,sizeof (char));
    myJson->recordDefultData->privateBuffer=(char *)calloc(100,sizeof (char));
    myJson->recordDeta->privateBuffer=(char *)calloc(100,sizeof (char));
    defultStruct();
    char data[100];
    /* File pointer to hold reference to our file */
    readFile();
}

void readFile(){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    myJson->fp = fopen("/home/chana/PROJECTS/later2", "r+");
    struct stat st;
    stat("/home/chana/PROJECTS/later2", &st);
    int size = st.st_size;
    if(size==0){
        writeFile();
        return;
    }
    myJson->buffer=(char*)calloc(size,sizeof (char));
    char * temp =(char*)calloc(size,sizeof (char));
    int x=0;
    while ((read = getline(&line, &len, myJson->fp)) != -1) {
             strncpy((myJson->buffer)+x,line,strlen(line));
             strncpy((temp)+x,line,strlen(line));
             x+=strlen(line);
           }
    printf("the buffer is: %s",myJson->buffer);
    fclose(myJson->fp);
    if (line)
        free(line);
    splitStruct();
}


void writeFile(){
        myJson->fp = fopen("/home/chana/PROJECTS/later2", "w+");
        strncpy(myJson->buffer,myJson->snapshotData->privateBuffer,strlen(myJson->snapshotData->privateBuffer));
        int x=strlen((myJson->snapshotData->privateBuffer));
        strncpy((myJson->buffer)+x+1,myJson->recordDeta->privateBuffer,strlen(myJson->recordDeta->privateBuffer));
        fputs(myJson->buffer, myJson->fp);
        fclose(myJson->fp);
        printf("sucsses\n");
}

void defultStruct(){

    defultSnapshot(myJson->snapshotDataDEFULT,myJson->snapshotData);
    defultRecord(myJson->recordDefultData,myJson->recordDeta);
}

void defultSnapshot(snapshot_tt * snapshotDataDEFULT,snapshot_tt* snapshotData){
    snapshotData->file_name="defultName";
    snapshotData->height=320;
    snapshotData->type=0;
    snapshotData->width=240;
    //snapshotData->privateBuffer=(char *)calloc(100,sizeof (char));

    sprintf(snapshotData->privateBuffer,"\"snapshot\": {\n\t\"file_name\":%s,\n\t\"height\":%d,\n\t\"type\":%d,\n\t\"width\":%d,\n}"
            ,snapshotData->file_name,snapshotData->height,snapshotData->type,snapshotData->width);

    snapshotDataDEFULT->file_name="defultName";
    snapshotDataDEFULT->height=320;
    snapshotDataDEFULT->type="JPG";
    snapshotDataDEFULT->width=240;
    //snapshotData->privateBuffer=(char *)calloc(100,sizeof (char));

    sprintf(snapshotDataDEFULT->privateBuffer,"\"snapshot\": {\n\t\"file_name\":%s,\n\t\"height\":%d,\n\t\"type\":%d,\n\t\"width\":%d,\n}"
            ,snapshotDataDEFULT->file_name,snapshotDataDEFULT->height,snapshotDataDEFULT->type,snapshotDataDEFULT->width);
}

void defultRecord(record_tt * recordDataDefult,record_tt* recordData){
    recordData->file_name="defultName";
    recordData->codec=0;
    recordData->fps=0;
    recordData->height=320;
    recordData->width=240;

    sprintf(recordData->privateBuffer,"\"record\": {\n\t\"file_name\":%s,\n\t\"codec\":%d,\n\t\"fps\":%d,\n\t\"height\":%d,\n\t\"width\":%d,\n}"
            ,recordData->file_name,recordData->codec,recordData->fps,recordData->height,recordData->width);

    recordDataDefult->file_name="defultName";
    recordDataDefult->codec=0;
    recordDataDefult->fps=0;
    recordDataDefult->height=320;
    recordDataDefult->width=240;

    sprintf(recordDataDefult->privateBuffer,"\"record\": {\n\t\"file_name\":%s,\n\t\"codec\":%d,\n\t\"fps\":%d,\n\t\"height\":%d,\n\t\"width\":%d,\n}"
            ,recordDataDefult->file_name,recordDataDefult->codec,recordDataDefult->fps,recordDataDefult->height,recordDataDefult->width);

}

char * splitBuffer(char * title){
    char* start=(char*)calloc(strlen(strstr(myJson->buffer,title)),sizeof(char));
    start=strstr(myJson->buffer,title);
    char* end=(char*)calloc(strlen(strchr(start,'}')),sizeof(char));
    end=strchr(start,'}');
    int x=end-start;
    char*str=(char*)calloc(strlen(start),sizeof(char));
    //str=strdup(start);
    strcpy(str,start);
    str[++x]='\0';
    return str;
}

void splitSnapShotFromFile(char * str){
    char**valuesArr=(char**)calloc(4,sizeof(char*));
    int i=0;
    const char s[2] = ",";
    char *token;
    token = strtok(str, s);
    while( token != NULL ) {
        char* value=strchr(token,':');
        value++;
        token = strtok(NULL, s);
        valuesArr[i++]=value;
    }

    myJson->snapshotData->file_name=valuesArr[0];
    myJson->snapshotData->height=atoi(valuesArr[1]);
    myJson->snapshotData->type=atoi(valuesArr[2]);
    myJson->snapshotData->width=atoi(valuesArr[3]);

    sprintf(myJson->snapshotData->privateBuffer,"\"snapshot\": {\n\t\"file_name\":%s,\n\t\"height\":%d,\n\t\"type\":%d,\n\t\"width\":%d,\n}"
            ,myJson->snapshotData->file_name,myJson->snapshotData->height,myJson->snapshotData->type,myJson->snapshotData->width);



}


void splitRecordFromFile(char * str){
    char**valuesArr=(char**)calloc(5,sizeof(char*));
    int i=0;
    const char s[2] = ",";
    char *token;
    token = strtok(str, s);
    while( token != NULL ) {
        char* value=strchr(token,':');
        value++;
        token = strtok(NULL, s);
        valuesArr[i++]=value;
    }

    myJson->recordDeta->file_name=valuesArr[0];
    myJson->recordDeta->codec=atoi(valuesArr[1]);
    myJson->recordDeta->fps=atoi(valuesArr[2]);
    myJson->recordDeta->height=atoi(valuesArr[3]);
    myJson->recordDeta->width=atoi(valuesArr[4]);

    sprintf(myJson->recordDeta->privateBuffer,"\"record\": {\n\t\"file_name\":%s,\n\t\"codec\":%d,\n\t\"fps\":%d,\n\t\"height\":%d,\n\t\"width\":%d,\n}"
            ,myJson->recordDeta->file_name,myJson->recordDeta->codec,myJson->recordDeta->fps,myJson->recordDeta->height,myJson->recordDeta->width);


}


void splitStruct(){
    splitSnapShotFromFile(splitBuffer("snapshot"));
    splitRecordFromFile(splitBuffer("record"));


}

void setWidthSnapshot(int x){
    myJson->snapshotData->width=x;
    sprintf(myJson->snapshotData->privateBuffer,"\"snapshot\": {\n\t\"file_name\":%s,\n\t\"height\":%d,\n\t\"type\":%d,\n\t\"width\":%d,\n}"
            ,myJson->snapshotData->file_name,myJson->snapshotData->height,myJson->snapshotData->type,myJson->snapshotData->width);

    writeFile();

}

record_tt getRecord(){
    return *(myJson->recordDeta);
}

snapshot_tt getSnapshot(){
    return *(myJson->snapshotData);
}
