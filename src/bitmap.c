#include "bitmap.h"
#include "stdlib.h"


void setDefaultFileHeader(BitmapFileHeader* h)
{

}


void loadBitmapFromFile(const char * str,Bitmap * bmp)
{
    FILE *file;
    char *tmp; 
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
    //for()
    printf("%d , %u \n",bmp->rowSize*bmp->height,bitmapInfoHeader.biSizeImage);

    //read in the bitmap image data
    ret = fread(bmp->data,bitmapInfoHeader.biSizeImage,1,file);
    if(!ret)
    {
        perror("fread");
    }
    
    fclose(file);
}
