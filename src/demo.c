/*
 * The main executable
 */
#include "demo.h"
#include<stdio.h>
#include "bitmap.h"

int main()
{
    Bitmap b;
    loadBitmapFromFile("/home/user/git/lena.bmp",&b);

    bitmapToBW(&b);
    saveBitmap(&b,"out.bmp");
    return 0;
}
