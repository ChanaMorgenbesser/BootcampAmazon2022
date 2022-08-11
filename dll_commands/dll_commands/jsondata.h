#ifndef CFG_H
#define CFG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct snapshott{
    char * privateBuffer;
    char* file_name;
    int width;
    int height;
    int type;
}snapshot_tt;

typedef struct record_tt{
    char * privateBuffer;
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_tt;


typedef struct json{
    char * buffer;
    char * fullPath;
    FILE * fp;
    snapshot_tt * snapshotData;
    snapshot_tt * snapshotDataDEFULT;
    record_tt * recordDeta;
    record_tt * recordDefultData;
}MyJson ;

static MyJson * myJson;

void initMyJson();
void readFile();
void writeFile();
void defultStruct();
void defultSnapshot(snapshot_tt *,snapshot_tt*);
void defultRecord(record_tt *,record_tt*);
void splitStruct();
char * splitBuffer(char *);
void splitSnapShotFromFile(char *);
void splitRecordFromFile(char *);
void setWidthSnapshot(int);
record_tt getRecord();
snapshot_tt getSnapshot();



#endif // CFG_H


