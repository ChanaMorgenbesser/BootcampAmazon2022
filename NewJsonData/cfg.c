#include "cfg.h"
#include <sys/stat.h>
#include <string.h>


void readFile()
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    //FILE * f=fopen("/home/chana/yyy","wr+");
    myJson->fp = fopen("/home/chana/Documents/BBBBGGGGG", "wr+");
    struct stat st;
    st.st_size=0;
    stat("/home/chana/Documents/BBBBGGGGG", &st);
    int size = st.st_size;
    if(size==0){
        fclose(myJson->fp);
        printf("the file empty!\n");

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

    splitStruct();
}

void writeFile(){

    myJson->fp = fopen("/home/chana/Documents/BBBBGGGGG", "wr+");
        /* fopen() return NULL if last operation was unsuccessful */
        if(myJson->fp == NULL)
        {
            /* File not created hence exit */
            printf("Unable to create file.\n");
            return;
        }

        int size=strlen(myJson->imageData->privateBuffer)+strlen(myJson->textData->privateBuffer)+strlen(myJson->windowData->privateBuffer);
        myJson->buffer=(char*)calloc(size+3,sizeof (char));
        int x=0;
        strncpy(myJson->buffer,myJson->imageData->privateBuffer,strlen(myJson->imageData->privateBuffer));
        x+=strlen(myJson->imageData->privateBuffer)+1;
        strncpy((myJson->buffer)+x,myJson->textData->privateBuffer,strlen(myJson->textData->privateBuffer));
        x+=strlen(myJson->textData->privateBuffer)+1;
        strncpy((myJson->buffer)+x,myJson->windowData->privateBuffer,strlen(myJson->windowData->privateBuffer));

        /* Write data to file */

        //freopen(myJson->fp);
        //freopen(NULL,'w',myJson->fp);

        fputs(myJson->buffer, myJson->fp);
        /* Close file to save file data */
        fclose(myJson->fp);
        /* Success message */
        printf("File created and saved successfully. :) \n");
}


void defultStruct(){

    defultImage(myJson->imageDataDEFULT,myJson->imageData);
    defultText(myJson->textDataDEFULT,myJson->textData);
    defultWindow(myJson->windowDataDEFULT,myJson->windowData);

}
void defultImage(Image * imageDefult,Image * imageData){

    imageData->alignment="center";
    imageData->hOffset=100;
    imageData->name="defultName";
    imageData->src="defultSRC";
    imageData->vOffset=150;
    imageData->privateBuffer=(char *)calloc(100,sizeof (char));

    sprintf(imageData->privateBuffer,"\"image\": {\n\t\"src\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n}"
            ,imageData->src,imageData->name,imageData->hOffset,imageData->vOffset,imageData->alignment);

    imageDefult->alignment="center";
    imageDefult->hOffset=100;
    imageDefult->name="defultName";
    imageDefult->src="defultSRC";
    imageDefult->vOffset=150;

    imageDefult->privateBuffer=(char *)calloc(100,sizeof (char));
    sprintf(imageDefult->privateBuffer,"\"image\": {\n\t\"src\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n}"
            ,imageDefult->src,imageDefult->name,imageDefult->hOffset,imageDefult->vOffset,imageDefult->alignment);

}
void defultText(Text * textDefult,Text* TextData){

    textDefult->alignment="center";
    textDefult->data="defultData";
    textDefult->hOffset=100;
    textDefult->name="defultName";
    textDefult->onMouseUp="defultOnMousUp";
    textDefult->size=100;
    textDefult->style="defultStyle";
    textDefult->vOffset=150;
    textDefult->privateBuffer=(char *)calloc(100,sizeof (char));

    sprintf(textDefult->privateBuffer,"\"text\": {\n\t\"data\":%s,\n\t\"size\":%d,\n\t\"style\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n\t\"onMouseUp\":%s,\n},"
            ,textDefult->data,textDefult->size,textDefult->style,textDefult->name,textDefult->hOffset,textDefult->vOffset,textDefult->alignment,textDefult->onMouseUp);

    TextData->alignment="center";
    TextData->data="defultData";
    TextData->hOffset=100;
    TextData->name="defultName";
    TextData->onMouseUp="defultOnMousUp";
    TextData->size=100;
    TextData->style="defultStyle";
    TextData->vOffset=150;
    //TextData->privateBuffer=(char *)malloc(80*sizeof (char));

    sprintf(TextData->privateBuffer,"\"text\": {\n\t\"data\":%s,\n\t\"size\":%d,\n\t\"style\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n\t\"onMouseUp\":%s,\n},"
            ,TextData->data,TextData->size,TextData->style,TextData->name,TextData->hOffset,TextData->vOffset,TextData->alignment,TextData->onMouseUp);
}
void defultWindow(Window * windowDefult,Window* WindowData){

    windowDefult->height=100;
    windowDefult->name="defultName";
    windowDefult->titlew="window";
    windowDefult->width=150;
    //windowDefult->privateBuffer=(char *)calloc(100,sizeof (char));

    sprintf(windowDefult->privateBuffer,"\"window\": {\n\t\"title\":%s,\n\t\"name\":%s,\n\t\"width\":%d,\n\t\"heigth\":%d,\n},"
            ,windowDefult->titlew,windowDefult->name,windowDefult->width,windowDefult->height);

    //WindowData->privateBuffer=(char *)calloc(100,sizeof (char));
    WindowData->height=100;
    WindowData->name="defultName";
    WindowData->titlew="window";
    WindowData->width=150;
    sprintf(WindowData->privateBuffer,"\"window\": {\n\t\"title\":%s,\n\t\"name\":%s,\n\t\"width\":%d,\n\t\"heigth\":%d,\n},"
            ,WindowData->titlew,WindowData->name,WindowData->width,WindowData->height);
}
void setVOffset(int x){
    myJson->imageData->vOffset=x;
    sprintf(myJson->imageData->privateBuffer,"\"image\": {\n\t\"src\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n}"
            ,myJson->imageData->src,myJson->imageData->name,myJson->imageData->hOffset,myJson->imageData->vOffset,myJson->imageData->alignment);
    writeFile();

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

void splitStruct(){

    splitImageFromFile(splitBuffer("image"));
    splitTextFromFile(splitBuffer("text"));
    splitWindowFromFile(splitBuffer("window"));
}
void splitTextFromFile(char * str){
    char**valuesArr=(char**)calloc(8,sizeof(char*));
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

    myJson->textData->data=valuesArr[0];
    myJson->textData->size=atoi(valuesArr[1]);
    myJson->textData->style=valuesArr[2];
    myJson->textData->name=valuesArr[3];
    myJson->textData->hOffset=atoi(valuesArr[4]);
    myJson->textData->vOffset=atoi(valuesArr[5]);
    myJson->textData->alignment=valuesArr[6];
    myJson->textData->onMouseUp=valuesArr[7];

    sprintf(myJson->textData->privateBuffer,"\"text\": {\n\t\"data\":%s,\n\t\"size\":%d,\n\t\"style\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n\t\"onMouseUp\":%s,\n},"
            ,myJson->textData->data,myJson->textData->size,myJson->textData->style,myJson->textData->name,myJson->textData->hOffset,myJson->textData->vOffset,myJson->textData->alignment,myJson->textData->onMouseUp);
}
void splitWindowFromFile(char * str){
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
    myJson->windowData->titlew=valuesArr[0];
    myJson->windowData->name=valuesArr[1];
    myJson->windowData->width=atoi(valuesArr[2]);
    myJson->windowData->height=atoi(valuesArr[3]);

    sprintf(myJson->windowData->privateBuffer,"\"window\": {\n\t\"title\":%s,\n\t\"name\":%s,\n\t\"width\":%d,\n\t\"heigth\":%d,\n},"
            ,myJson->windowData->titlew,myJson->windowData->name,myJson->windowData->width,myJson->windowData->height);


}


void splitImageFromFile(char * str){
    char**valuesArr=(char**)calloc(5,sizeof(char*));
    int i=0;
      const char s[2] = ",";
      char *token;
      char* value=strchr(str,':');
      token = strtok(str, s);
      while( token != NULL ) {
          value=strchr(token,':');
         value++;
         token = strtok(NULL, s);
         valuesArr[i++]=value;

      }
    myJson->imageData->src=valuesArr[0];
    myJson->imageData->name=valuesArr[1];
    myJson->imageData->hOffset=atoi(valuesArr[2]);
    myJson->imageData->vOffset=atoi(valuesArr[3]);
    myJson->imageData->alignment=valuesArr[4];
    sprintf(myJson->imageData->privateBuffer,"\"image\": {\n\t\"src\":%s,\n\t\"name\":%s,\n\t\"hOffset\":%d,\n\t\"vOffset\":%d,\n\t\"alignment\":%s,\n}"
            ,myJson->imageData->src,myJson->imageData->name,myJson->imageData->hOffset,myJson->imageData->vOffset,myJson->imageData->alignment);

}






void initMyJson(){



    myJson=(MyJson*)calloc(1,sizeof (MyJson));

  // MyJson * myJson=(MyJson*)calloc(1,sizeof (MyJson));

    char * p="/home/chana/Documents/BBBBGGGGG";
    myJson->fullPath=(char*)calloc(strlen(p),sizeof (char));
    strcpy(myJson->fullPath,p);
    myJson->imageData=(Image *)calloc(1,sizeof (Image));

    myJson->imageDataDEFULT=(Image *)calloc(1,sizeof (Image));
    myJson->textData=(Text *)calloc(1,sizeof (Text));
    myJson->textDataDEFULT=(Text *)calloc(1,sizeof (Text));
    myJson->windowData=(Window *)calloc(1,sizeof (Window));
    myJson->windowDataDEFULT=(Window *)calloc(1,sizeof (Window));
    myJson->textData->privateBuffer=(char *)calloc(100,sizeof (char));
    myJson->windowData->privateBuffer=(char *)calloc(100,sizeof (char));
    myJson->windowDataDEFULT->privateBuffer=(char *)calloc(100,sizeof (char));







    defultStruct();


    char data[100];
        /* File pointer to hold reference to our file */
    readFile();


}



















