/*
 * The main executable
 */
#include "demo.h"
#include<stdio.h>
#include "bitmap.h"

int main()
{
    Bitmap b;
    Pixel p;
    loadBitmapFromFile("/home/vighnesh/bitmaps/nature.bmp",&b);
    p = bitmapGetPixel(&b,123,100);
    printf("%d %d %d\n",p.red,p.green,p.blue);
    saveBitmap(&b,"out.bmp");
    
    return 0;
}
