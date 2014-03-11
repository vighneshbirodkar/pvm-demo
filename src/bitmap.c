#include "bitmap.h"
#include<math.h>
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
    p.red = 0;
    p.green = 0;
    p.blue = 0;
    //printf("%d,%d\n",x,y);
    if( x < 0)
        return p;
    if ( y < 0 )
        return p;
    if(x >= bmp->width)
        return p;
    if(y >= bmp->height)
        return p;
    //printf("value = %d\n",p.red);
    
    y = bmp->height - y - 1;
    p.blue = bmp->data[y*bmp->rowSize + 3*x];
    p.green = bmp->data[y*bmp->rowSize + 3*x + 1];
    p.red = bmp->data[y*bmp->rowSize + 3*x + 2];
    return p;
}

void bitmapSetPixel(Bitmap *bmp,int x,int y,Pixel p)
{

    if( x < 0)
        return ;
    if ( y < 0 )
        return ;
    if(x >= bmp->width)
        return ;
    if(y >= bmp->height)
        return ;

    y = bmp->height - y - 1;
    bmp->data[y*bmp->rowSize + 3*x]=p.blue;
    bmp->data[y*bmp->rowSize + 3*x + 1]=p.green;
    bmp->data[y*bmp->rowSize + 3*x +2]=p.red;
}
//converts rgb image to gray scale
void bitmapToBW(Bitmap *bmp)
{
    float r,g,b,f;
    Pixel p;
    int i,j;

    for(i=0;i<bmp->width;i++)
    {
        for(j=0;j<bmp->height;j++)
        {
            p = bitmapGetPixel(bmp,i,j);
            r = p.red;
            g = p.green;
            b = p.blue;
            f = r*.29 + g*.59 + b*.11;
            p.red = (unsigned char)f;
            p.blue = (unsigned char)f;
            p.green = (unsigned char)f;
            bitmapSetPixel(bmp,i,j,p);
        }
    }
}
//defines a new kernel
void newKernel(Kernel* kr,int kSize)
{
    if(kSize%2==0)
    {
        printf("kernel should be of odd size");
        //exit(1);
    }
    kr->kSize =  kSize;
    //printf("rowsize: %d datasize %d\n'",kr->krowSize,kr->kdataSize );
    kr->kdata = (float*)malloc(kSize*kSize*sizeof(float));
    bzero(kr->kdata,kSize*kSize*sizeof(float));
    //printf("in kernel\n");
}

//set a value for a specific position
void kernelSetValue(Kernel* kr,int x,int y,float value)
{
    int s = kr->kSize/2;
    
    if( (x < -s) || (x > s) || (y < -s) || (y > s) )
    {
        return ;
    }
    x += s;
    y += s;
    kr->kdata[y*kr->kSize + x] = value;
}
// get the value of a particular kernel position
float kernelGetValue(Kernel* kr,int x,int y)
{
    float value = 1;
    int s = kr->kSize/2;
    
    if( (x < -s) || (x > s) || (y < -s) || (y > s) )
    {
        return 0;
    }
    x += s;
    y += s;
    value = kr->kdata[x*kr->kSize + y];
    return value;
}

//print the kernel values
void kernelPrint(Kernel* kr)
{
    int i,j;
    int s = kr->kSize/2;
    for(i= -s; i<=s; i++)
    {
        for(j=-s; j<=s; j++)
        {
            printf("%f\t",kernelGetValue(kr,i,j));
        }
        printf("\n");
        
    }

}
//perform convolution of bmp image with kernel
void convolve(Bitmap* bmp,Kernel* kr,Bitmap* out)
{
    if(bmp->width != out->width)
        perror("Damn, I cant do that\n");
    if(bmp->height != out->height)
        perror("Damn, I cant do that\n");
    int i,j;
    int ki,kj;
    int s = kr->kSize/2;
    float sumr,sumb,sumg;
    Pixel pixel;
    
    for(i=0;i<bmp->width;i++)
    {
        for(j=0;j<bmp->height;j++)
        {
            //pixel = bitmapGetPixel(bmp,i,j)
            sumr = sumg = sumb = 0;
            for(ki=-s ; ki <=s ; ki++)
            {
                for(kj=-s ; kj <=s ; kj++)
                {
                    pixel = bitmapGetPixel(bmp,i+ki,j+kj);
                    sumr += pixel.red*kernelGetValue(kr,ki,kj);
                    sumg += pixel.green*kernelGetValue(kr,ki,kj);
                    sumb += pixel.blue*kernelGetValue(kr,ki,kj);
                }
            }
            pixel.red = sumr;
            pixel.green = sumg;
            pixel.blue = sumb;
            bitmapSetPixel(out,i,j,pixel);
        }
    }
}

//inserts values for kernel using 2D gaussian function  with mean 0,0 and variance = var
float gaussKernel(Kernel* kr,int var)
{
    int i,j;
    float expi,value, c, sum=0.0;
    int s = kr->kSize/2;
    for(i= -s; i<=s; i++)
    {
        for(j=-s; j<=s; j++)
        {
            expi = (-((float)(i*i)+(j*j))/(2*var*var));
            c= exp(expi);
            value = ((1/(2*PI*var*var))*c);
            kernelSetValue(kr,i,j,value);
            sum += value;
            //printf("i=%d j=%d value=%f \n",i,j,value);
        }
        //printf("\n");
    }
    //printf("sum=%f\n", sum);
    return sum;
}

// does normalisation of kernel values by sum of all kernel values
void normKernel(Kernel* kr, float norm)
{
    int i,j;
    float a,b;
    int s = kr->kSize/2;
    for(i= -s; i<=s; i++)
    {
/*        a=kernelGetValue(kr,i,i);*/
/*        b=a/norm;*/
/*        kernelSetValue(kr,i,i,b);*/
        for(j=-s; (j<=s && i!=j); j++)
        {
            a=kernelGetValue(kr,i,j);
            b=a/norm;
            kernelSetValue(kr,i,j,b);
        }
    }
    for(i= -s; i<=s; i++)
    {
        a=kernelGetValue(kr,i,i);
        b=a/norm;
        kernelSetValue(kr,i,i,b);
    }//printf("last\n");
}

//img division
void img_division(Bitmap* bmp, int procno, int ksize)
{
     Bitmap i1;
    int rheight = bmp->height/procno;
    int rht=0;
    Pixel p;
    int i,j=0,m, k;
    char a[100] ="out";
    char b[100] = "";
    char ab[100] = "";
    for(i=0;i<procno;i++)
    {
        newBitmap( &i1,bmp->width,rheight);
        for(k=0;k<i1.width;k++)
        {
            for(j=rht;j<rht+rheight;j++)
            {
                m = j - (rht);
                //printf("m=%d \n",m);
                p = bitmapGetPixel(bmp,k,j);
                bitmapSetPixel(&i1,k,m,p);
             }
        }
        //printf("%d \n",j);
        rht=rht+rheight;
        //printf("proc no %d \trht %d\n", i, rht);
        sprintf(ab,"%s%d", a ,i);
        strcpy(b,ab);
        strcat(b,".bmp");
        saveBitmap(&i1,b);
    }
}
//TODO
//modify img division for the nth proc ie to handle the case img ht% proc no is not 0
