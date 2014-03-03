#include "bitmap.h"
#include<stdlib.h>
#include<strings.h>


void setDefaultFileHeader(BitmapFileHeader* h)
{
    h->bfType = 0x4d42;

    //Bitmap size + 54
    h->bfSize = 0x00;
    h->bfReserved1 = 0;
    h->bfReserved2 = 0;
    h->bOffBits = 0x36;
}

void setDefaultInfoHeader(BitmapInfoHeader* h)
{
    h->biSize = 0x28;
    //Must be updated later
    h->biWidth = 0x00;
    h->biHeight = 0x00;
    h->biPlanes = 0x01;
    h->biBitCount = 0x18; //or 24 :-)
    h->biCompression = 0x00;
    //Must be updated later
    h->biSizeImage = 0x00;
    h->biXPelsPerMeter = 0xb13;
    h->biYPelsPerMeter = 0xb13;
    h->biClrUsed = 0;
    h->biClrImportant = 0;
    
}

void loadBitmapFromFile(const char * str,Bitmap * bmp)
{
    FILE *file;
    BitmapFileHeader bitmapFileHeader; 
    BitmapInfoHeader bitmapInfoHeader;
    int ret;

    file = fopen(str,"rb");
    if (file == NULL)
    {
        perror("Damn ! I can't open that file");
        return;
    }
    
    //read the bitmap file header
    ret = fread(&bitmapFileHeader, sizeof(BitmapFileHeader),1,file);
    if(!ret)
    {
        perror("fread");
        return;
    }
    
    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        perror("This file probably isn't a Bitmap, are you sure ?");
        fclose(file);
    }

    ret = fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader),1,file); 
    if(!ret)
    {
        perror("fread");
        return;
    }

    bmp->width = bitmapInfoHeader.biWidth;
    bmp->height = bitmapInfoHeader.biHeight;
    bmp->fileHeader = bitmapFileHeader;
    bmp->infoHeader = bitmapInfoHeader;
    
    //move file point to the begging of bitmap data
    fseek(file, bitmapFileHeader.bOffBits, SEEK_SET);

    bmp->data = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
    //Assuming only 24bit images
    bmp->rowSize = (((24*bmp->width+31)/32)*4);
    bmp->dataSize = bitmapInfoHeader.biSizeImage;

    //read in the bitmap image data
    ret = fread(bmp->data,bitmapInfoHeader.biSizeImage,1,file);
    if(!ret)
    {
        perror("fread");
    }
    
    fclose(file);
}

void newBitmap(Bitmap* bmp,int width,int height)
{
    
    setDefaultFileHeader(&bmp->fileHeader);
    setDefaultInfoHeader(&bmp->infoHeader);
    
    bmp->width = width;
    bmp->height = height;
    bmp->rowSize = (((24*bmp->width+31)/32)*4);
    bmp->dataSize = bmp->rowSize*bmp->height;
    
    bmp->infoHeader.biWidth = width;
    bmp->infoHeader.biHeight = height;
    bmp->infoHeader.biSizeImage = bmp->rowSize*bmp->height;
    
    bmp->fileHeader.bfSize = 54 + bmp->infoHeader.biSizeImage;
    
    bmp->data = (unsigned char*)malloc(bmp->dataSize);
    bzero(bmp->data,bmp->dataSize);
}

void saveBitmap(Bitmap* bmp,const char * str)
{
    FILE* file;
    
    file = fopen(str,"w");
    if(!file)
    {
        perror("Damn Son ! I can't write there");
        return;
    }
    
    fwrite(&bmp->fileHeader,sizeof(BitmapFileHeader),1,file);
    fwrite(&bmp->infoHeader,sizeof(BitmapInfoHeader),1,file);
    fwrite(bmp->data,bmp->dataSize,1,file);
    
    fclose(file);
}

Pixel bitmapGetPixel(Bitmap* bmp,int x,int y)
{
    Pixel p;
    y = bmp->height - y - 1;
    p.blue = bmp->data[y*bmp->rowSize + 3*x];
    p.green = bmp->data[y*bmp->rowSize + 3*x + 1];
    p.red = bmp->data[y*bmp->rowSize + 3*x + 2];
    return p;
}

void setPixel(Bitmap *bmp,int x,int y,Pixel p)
{
    y = bmp->height - y - 1;
    bmp->data[y*bmp->rowSize + 3*x]=p.blue;
    bmp->data[y*bmp->rowSize + 3*x + 1]=p.green;
    bmp->data[y*bmp->rowSize + 3*x +2]=p.red;
}



