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
    bitmapToBW(&b);
    newBitmap(&o,b.width,b.height);
    Kernel k;
    int i,j;
    int size = 11;
    int size2 = size/2;
    newKernel(&k,size);
    float f;
    /*
    kernelSetValue(&k,-1,-1,-.125);
    kernelSetValue(&k,-1,0,-.25);
    kernelSetValue(&k,-1,1,-.125);
    kernelSetValue(&k,1,1,.125);
    kernelSetValue(&k,1,0,.25);
    kernelSetValue(&k,1,-1,.125);*/
    
    f = 1/((float)size*size);
    for(i=-size2;i<=size2;i++)
    {
        for(j=-size2;j<=size2;j++)
        {
            kernelSetValue(&k,i,j,f);
        }
    }
    
    kernelPrint(&k);
    convolve(&b,&k,&o);
    saveBitmap(&o,"out.bmp");
    return 0;
}
