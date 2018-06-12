#ifndef PARSING_H
#define PARSING_H

#include <stdbool.h>

//By reference string, pointer gets incremented by parse functions
//They return true on success, false on failure

bool parseInt(const char** source, int* out);
bool parseDouble(const char** source, double* out);
bool parseWord(const char** source, char* out);
void getLine(const char** source, char* out);

bool expectString(const char** source, const char* word);
bool expectLineEnd(const char** source);

void skipWhitespace(const char** source);
void skipWhitespaceInLine(const char** source);
bool skipWhitespaceExpectLineEnd(const char** source);

#endif