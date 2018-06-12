#include "GameFunction.h"

#include <malloc.h>
#include <math.h>


GameFunction* GameFunction_construct(int paramCount)
{
    GameFunction* gfunc = (GameFunction*)malloc(sizeof(GameFunction));
    gfunc->params = (double*)malloc(sizeof(double) * paramCount);
    return gfunc;
}

double GameFunction_call(GameFunction* gfunc, double x)
{
    return gfunc->func(gfunc->params, x - 1);
}

void GameFunction_delete(GameFunction* gfunc)
{
    free(gfunc->params);
    free(gfunc);
}

double constFunc(double* params, double x)
{
    return params[0];
}

double linearFunc(double* params, double x)
{
    return params[0] + params[1] * x;
}

double quadraticFunc(double* params, double x)
{
    return params[0] + params[1] * x * x;
}

double powFunc(double* params, double x)
{
    return params[0] + params[1] * pow(x, params[2]);
}

double expFunc(double* params, double x)
{
    return params[0] + params[1] * pow(params[2], x);
}

double logFunc(double* params, double x)
{
    return params[0] + params[1] * (log(params[2]+x) / log(params[2]));
}