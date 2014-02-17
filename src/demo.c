/*
 * The main executable
 */
#include "demo.h"
#include<stdio.h>
#include "bitmap.h"

int main()
{
    Bitmap b;
    unsigned char c1,c2,c3;
    int x=3,y=2;
    int wd;
    loadBitmapFromFile("/home/vighnesh/bitmaps/demo.bmp",&b);
    wd = b.width;
    //x--;
    //y--;
    //printf("size = %u",b.infoHeader.biSize);
    for(x=0;x<5;x++)
    {
        for(y=0;y<5;y++)
        {
            printf("x = %d y = %d\n",x,y);
            c1 = b.data[b.rowSize*y+3*x];
            c2 = b.data[b.rowSize*y+3*x+1];
            c3 = b.data[b.rowSize*y+3*x+2];
            printf("%d %d %d \n",c1,c2,c3);
        }
    }

    return 0;
}
