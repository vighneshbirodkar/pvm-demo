/*
 * The main executable
 */
#include "demo.h"
#include<stdio.h>
#include "bitmap.h"

int main()
{
    Bitmap b;
    Bitmap o,i1;
    loadBitmapFromFile("/home/esha/git/pvm-demo/lena.bmp",&b);
    newBitmap(&o,b.width,b.height);
    Kernel k;
    float sum;
    int size = 5;
    newKernel(&k,size);
    sum = gaussKernel(&k, 4);
    kernelPrint(&k);
    normKernel(&k, sum);
    printf("now do norm with sum %f \n", sum);
    kernelPrint(&k);
    convolve(&b,&k,&o);
    saveBitmap(&o,"out.bmp");
    
    return 0;
}
