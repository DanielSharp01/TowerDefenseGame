#include "Parsing.h"

#include <ctype.h>
#include <stdio.h>

bool parseInt(const char** source, int* out)
{
    int n;
    if (sscanf(*source, "%d%n", out, &n) == 1)
    {
        *source += n;
        return true;
    }
    else return false;
}


bool parseDouble(const char** source, double* out)
{
    int n;
    if (sscanf(*source, "%lf%n", out, &n) == 1)
    {
        *source += n;
        return true;
    }
    else return false;
}

bool parseWord(const char** source, char* out)
{
    int n;
    if (sscanf(*source, "%s%n", out, &n) == 1)
    {
        *source += n;

        return true;
    }
    else return false;
}

void getLine(const char** source, char* out)
{
    while (**source != '\n' && **source != '\0')
    {
        *out++ = *(*source)++;
    }
    *out = '\0';
}

bool expectString(const char** source, const char* word)
{
    int backtrack = 0;
    while (**source != '\0' && *word != '\0' && **source == *word)
    {
        (*source)++;
        backtrack++;
        word++;
    }

    if (*word == '\0')
        return true;
    else
    {
        (*source) -= backtrack;
        return false;
    }
}

bool expectLineEnd(const char** source)
{
    if (**source == '\n')
    {
        (*source)++;
        return true;
    }

    return **source == '\0';
}

void skipWhitespace(const char** source)
{
    while (isspace(**source))
        (*source)++;
}

void skipWhitespaceInLine(const char** source)
{
    while (**source != '\n' && isspace(**source))
        (*source)++;
}

bool skipWhitespaceExpectLineEnd(const char** source)
{
    skipWhitespaceInLine(source);
    return expectLineEnd(source);
}