#ifndef __BITMAP_H__
#define __BITMAP_H__


#include <stdint.h>
#include<stdio.h>
/*
 * Ensure no padding between struct members
 * Necessary to load data into struct from file
 */
#pragma pack(push, 1)

typedef struct BitmapFileHeader
{
    uint16_t bfType;  //specifies the file type
    uint32_t bfSize;  //specifies the size in bytes of the bitmap file
    uint16_t bfReserved1;  //reserved; must be 0
    uint16_t bfReserved2;  //reserved; must be 0
    uint32_t bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;


typedef struct BitmapInfoHeader
{
    uint32_t biSize;  //specifies the number of bytes required by the struct
    uint32_t biWidth;  //specifies width in pixels
    uint32_t biHeight;  //species height in pixels
    uint16_t biPlanes; //specifies the number of color planes, must be 1
    uint16_t biBitCount; //specifies the number of bit per pixel
    uint32_t biCompression;//spcifies the type of compression
    uint32_t biSizeImage;  //size of image in bytes
    uint32_t biXPelsPerMeter;  //number of pixels per meter in x axis
    uint32_t biYPelsPerMeter;  //number of pixels per meter in y axis
    uint32_t biClrUsed;  //number of colors used by th ebitmap
    uint32_t biClrImportant;  //number of colors that are important
}BitmapInfoHeader;

#pragma pack(pop)

typedef struct Bitmap 
{
    int height;
    int width;
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    unsigned char *data;
    unsigned int rowSize;
    unsigned int dataSize;
} Bitmap;

typedef struct Pixel 
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}Pixel;

void setDefaultFileHeader(BitmapFileHeader* h);
void setDefaultInfoHeader(BitmapInfoHeader* h);
void loadBitmapFromFile(const char * str,Bitmap * bmp);
void newBitmap(Bitmap* bmp,int width,int height);
void saveBitmap(Bitmap* bmp,const char * str);

void setPixel(Bitmap *bmp,int x,int y,Pixel p);
Pixel bitmapGetPixel(Bitmap* bmp,int x,int y);

#endif
