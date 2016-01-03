#ifndef _DCT_H_
#define _DCT_H_
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#define PI 3.141592653589793
#define SQH 0.707106781186547 /* square root of 2 */
#define SWAP(a,b)  tempr=(a); (a) = (b); (b) = tempr

void dct1(float *x, int n);
void idct1(float *x, int n);
static void fft1(float *data, int nn, int isign);

void dct2(float **x, int n);
void idct2(float **x, int n);

#endif
