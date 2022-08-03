
#ifndef FUNCTION_H
#define FUNCTION_H
typedef struct func{
    void (*command)(char*);
    char* name_function;
    char* f_help;
    struct func *next;
}Func;
void start_record(char*);
void start_record_cli(int , char* ,double ,int );
void stop_record(char*);
void get_status(char*);
char** splitArg(char*,int);
int validParam(char *,char*);
Func* reset();
Func*  registerFunc(Func** , char* ,void(*command)(char*),char* );
#endif // FUNCTION_H
