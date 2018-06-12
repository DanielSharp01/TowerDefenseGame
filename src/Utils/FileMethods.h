#ifndef FILE_METHODS_H
#define FILE_METHODS_H

//NOTE: Pointers have to be freed up by callee
char* readText(const char* path);
unsigned char* readImage(const char* path, int* width, int* height);
void freeImage(unsigned char* rawImage);

//Allocated by callee
char* getDirectoryName(char* dst, const char* src);

#endif