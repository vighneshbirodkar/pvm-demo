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
    p.red = 0;
    p.blue = 255;
    p.green = 0;
    int i,j;
    loadBitmapFromFile("/home/user/git/lena.bmp",&b);

    for(i=20;i<400;i++)
    {
        for(j=200;j<240;j++)
        {
            setPixel(&b,i,j,p);
        }
    }
    saveBitmap(&b,"out.bmp");
    return 0;
}
