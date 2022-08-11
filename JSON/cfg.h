#ifndef CFG_H
#define CFG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct snapshot{
    char * privateBuffer;
    char* file_name;
    int width;
    int height;
    int type;
}snapshot_t;

typedef struct record_t{
    char * privateBuffer;
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_t;


typedef struct json{
    char * buffer;
    char * fullPath;
    FILE * fp;
    snapshot_t * snapshotData;
    snapshot_t * snapshotDataDEFULT;
    record_t * recordDeta;
    record_t * recordDefultData;
}MyJson ;

static MyJson * myJson;

void initMyJson();
void readFile();
void writeFile();
void defultStruct();
void defultSnapshot(snapshot_t *,snapshot_t*);
void defultRecord(record_t *,record_t*);
void splitStruct();
char * splitBuffer(char *);
void splitSnapShotFromFile(char *);
void splitRecordFromFile(char *);
void setWidthSnapshot(int);
record_t getRecord();
snapshot_t getSnapshot();



#endif // CFG_H


