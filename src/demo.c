/*
 * The main executable
 */
#include "demo.h"
#include<stdio.h>
#include "bitmap.h"

int main()
{
    Bitmap b;
    Bitmap o;
    loadBitmapFromFile("/home/esha/git/pvm-demo/lena.bmp",&b);
    newBitmap(&o,b.width,b.height);
    Kernel k;
    float sum;
    int var, procno =5;
    int size;
    //input the variance of the gaussian
    //size of kernel is fixed to thrice the variance
    printf("Enter the variance \n");
    scanf("%d",&var);
    if(var%2==0)
    {
        size = var*3-1;
    }
    else
    {
        size = var*3;
    }
    newKernel(&k,size);
    sum = gaussKernel(&k, var);
    //kernelPrint(&k);
    normKernel(&k, sum);
    //printf("now do norm with sum %f \n", sum);
    kernelPrint(&k);
    convolve(&b,&k,&o);
    img_division(&b, procno, size);
    //saveBitmap(&o,"out.bmp");
    return 0;
}
