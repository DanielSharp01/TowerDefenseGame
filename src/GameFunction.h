#ifndef GAME_FUNCTION_H
#define GAME_FUNCTION_H

typedef double (*FuncPtr)(double* params, double x);

typedef struct 
{
    double* params;
    FuncPtr func;
} GameFunction;

GameFunction* GameFunction_construct(int paramCount);
double GameFunction_call(GameFunction* gfunc, double x);
void GameFunction_delete(GameFunction* gfunc);

double constFunc(double* params, double x);
double linearFunc(double* params, double x);
double quadraticFunc(double* params, double x);
double powFunc(double* params, double x);
double expFunc(double* params, double x);
double logFunc(double* params, double x);

#endif