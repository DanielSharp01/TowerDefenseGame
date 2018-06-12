#ifndef MULEARRAY_H
#define MULEARRAY_H

void*** MuleArray_create2DPointer(int w, int h);
void MuleArray_delete2DPointer(void*** arr);

//Generic method to create any 2D array with elementSize amount of bytes
//NOTE: Typecasted by callee
void** MuleArray_create2D(int w, int h, int elementSize);
void MuleArray_delete2D(void** arr);

#endif