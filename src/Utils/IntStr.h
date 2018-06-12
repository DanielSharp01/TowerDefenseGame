#ifndef INT_STR_H
#define INT_STR_H

int IntStr_length(int* istr);

int* IntStr_fromCStr(int* dst, char* app);
int* IntStr_fromInt(int* dst, int i);

int* IntStr_append(int* dst, int* app);
int* IntStr_appendCStr(int* dst, char* app);
int* IntStr_appendInt(int* dst, int i);

#endif