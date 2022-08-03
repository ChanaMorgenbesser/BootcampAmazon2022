
#ifndef CFG_H
#define CFG_H
#include <stdio.h>


typedef struct image{
    char * privateBuffer;
    char* src;
    char* name;
    int hOffset;
    int vOffset;
    char* alignment;

}Image ;


typedef struct text{
    char * privateBuffer;
    char* data;
    int size;
    char* style;
    char* name;
    int hOffset;
    int vOffset;
    char* alignment;
    char* onMouseUp;
}Text ;

typedef struct window{
    char * privateBuffer;
    char* titlew;
    char* name;
    int width;
    int height;
}Window ;

typedef struct json{
    char * buffer;
    char * fullPath;
    FILE * fp;
    Image * imageData;
    Text * textData;
    Window * windowData;
    Image * imageDataDEFULT;
    Text * textDataDEFULT;
    Window * windowDataDEFULT;
}MyJson ;


static MyJson * myJson;

void initMyJson();
void defultStruct();
void defultImage(Image *,Image*);
void defultText(Text *,Text*);
void defultWindow(Window *,Window*);
void readFile();
void splitStruct();
char * splitBuffer(char *);
void splitImageFromFile(char *);
void splitTextFromFile(char *);
void splitWindowFromFile(char *);

void writeFile();
void setVOffset(int);
//void splitStruct();



#endif // CFG_H

