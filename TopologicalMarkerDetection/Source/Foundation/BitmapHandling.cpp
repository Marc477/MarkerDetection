#include "BitmapHandling.h"


struct BitmapFileHeader
{
    BitmapFileHeader()
        :   bfType(19778)
        ,   bfSize(0)
        ,   bfReserved1(0)
        ,   bfReserved2(0)
        ,   bfOffBits(0)
    {}

    void Read(FILE* pFile)
    {
        fread(&bfType,      sizeof(unsigned short), 1,pFile);
        fread(&bfSize,      sizeof(unsigned long),  1,pFile);
        fread(&bfReserved1, sizeof(unsigned short), 1,pFile);
        fread(&bfReserved2, sizeof(unsigned short), 1,pFile);
        fread(&bfOffBits,   sizeof(unsigned long),  1,pFile);
    }

    void Write(FILE* pFile)
    {
        fwrite(&bfType,     sizeof(unsigned short), 1,pFile);
        fwrite(&bfSize,     sizeof(unsigned long),  1,pFile);
        fwrite(&bfReserved1,sizeof(unsigned short), 1,pFile);
        fwrite(&bfReserved2,sizeof(unsigned short), 1,pFile);
        fwrite(&bfOffBits,  sizeof(unsigned long),  1,pFile);
    }

    unsigned short  bfType;
    unsigned long   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long   bfOffBits;
} ;

struct BitmapInfoHeader
{
    BitmapInfoHeader()
        :    biSize(40),biWidth(0),biHeight(0),biPlanes(1),biBitCount(24)
        ,    biCompression(0),    biSizeImage(0),    biXPelsPerMeters(0)
        ,    biYPelsPerMeters(0), biClrUsed(0),    biClrImportant(0)
    {}

    //Read and Write are implemented independently since
    //the windows' BitmapInfoHeader uses platform specific
    //compiler byte alignement tricks that are hardly portable.

    void Read(FILE* pFile)
    {
        fread(&biSize,          sizeof(unsigned long),  1,pFile);
        fread(&biWidth,         sizeof(long),           1,pFile);
        fread(&biHeight,        sizeof(long),           1,pFile);
        fread(&biPlanes,        sizeof(unsigned short), 1,pFile);
        fread(&biBitCount,      sizeof(unsigned short), 1,pFile);
        fread(&biCompression,   sizeof(unsigned long),  1,pFile);
        fread(&biSizeImage,     sizeof(unsigned long),  1,pFile);
        fread(&biXPelsPerMeters,sizeof(long),           1,pFile);
        fread(&biYPelsPerMeters,sizeof(long),           1,pFile);
        fread(&biClrUsed,       sizeof(unsigned long),  1,pFile);
        fread(&biClrImportant,  sizeof(unsigned long),  1,pFile);
    }

    void Write(FILE* pFile)
    {
        fwrite(&biSize,             sizeof(unsigned long),  1,pFile);
        fwrite(&biWidth,            sizeof(long),           1,pFile);
        fwrite(&biHeight,           sizeof(long),           1,pFile);
        fwrite(&biPlanes,           sizeof(unsigned short), 1,pFile);
        fwrite(&biBitCount,         sizeof(unsigned short), 1,pFile);
        fwrite(&biCompression,      sizeof(unsigned long),  1,pFile);
        fwrite(&biSizeImage,        sizeof(unsigned long),  1,pFile);
        fwrite(&biXPelsPerMeters,   sizeof(long),           1,pFile);
        fwrite(&biYPelsPerMeters,   sizeof(long),           1,pFile);
        fwrite(&biClrUsed,          sizeof(unsigned long),  1,pFile);
        fwrite(&biClrImportant,     sizeof(unsigned long),  1,pFile);
    }

    unsigned long   biSize;
    long            biWidth;
    long            biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    long            biXPelsPerMeters;
    long            biYPelsPerMeters;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
};


void SaveBmp(const string &aFileName,const Image<ColorRGB> &aImage)
{
    FILE* pFile = fopen(aFileName.c_str(),"wb+");
    
    BitmapFileHeader oFileHeader;
    BitmapInfoHeader oInfoHeader;

    oInfoHeader.biWidth  = aImage.GetWidth();
    oInfoHeader.biHeight = aImage.GetHeight();

    //We compute stride sizes. A stride must be a factor
    //of 4.
    int StrideSize = oInfoHeader.biWidth * 3;
    if((oInfoHeader.biWidth*3)%4)
    {
        StrideSize += 4-((oInfoHeader.biWidth*3)%4);
    }

    //Only 24 bit mot is encoded (1 char per color components)
    //other modes require a palette, which would be a big more
    //complicated here.
    oInfoHeader.biBitCount  = 24;
    oInfoHeader.biSizeImage = StrideSize * oInfoHeader.biHeight;
    
    //Offset & size. (File header = 14 bytes, File info = 40 bytes)
    oFileHeader.bfOffBits = 54;
    oFileHeader.bfSize =    54 + sizeof(char) * 
                            StrideSize * 
                            oInfoHeader.biHeight;

    unsigned char * pBuffer = new unsigned char[StrideSize];

    //Write header information in the bmp file.
    oFileHeader.Write(pFile);
    oInfoHeader.Write(pFile);

    //Write raster data in the bmp file.
    //The y axis is inverted in bmp files, this is why we're
    //copying lines backward.
    for(int i = oInfoHeader.biHeight-1 ;i >=0; --i)
    {
        for(int j = 0; j < oInfoHeader.biWidth; ++j)
        {
            ColorRGB PixelColor = 
                aImage.GetRasterData()[i*oInfoHeader.biWidth + j];
            PixelColor.Clamp();

            pBuffer[j*3]    =    unsigned char(PixelColor.B()*255);
            pBuffer[j*3+1]  =    unsigned char(PixelColor.G()*255);
            pBuffer[j*3+2]  =    unsigned char(PixelColor.R()*255);
        }
        fwrite(pBuffer,StrideSize,1,pFile);
    }

    delete[] pBuffer;

    fclose(pFile);
}

void LoadBmp(const string &aFileName, Image<ColorRGB> &aImage)
{
    FILE* pFile = fopen(aFileName.c_str(),"rb+");

    BitmapFileHeader oFileHeader;
    BitmapInfoHeader oInfoHeader;

    //Read header information
    oFileHeader.Read(pFile);
    oInfoHeader.Read(pFile);

    if(oInfoHeader.biBitCount != 24)
    {
        fprintf(stdout,"Bitmaps with bit count different than \
                        24 are not supported.\n");
        fclose(pFile);
        return;
    }

    aImage.Resize(oInfoHeader.biWidth,oInfoHeader.biHeight);

    int StrideSize = (oFileHeader.bfSize-54)/(oInfoHeader.biHeight*sizeof(char));

    unsigned char *tBuffer = new unsigned char[StrideSize];

    for(int i = 0; i < oInfoHeader.biHeight; ++i)
    {
        fread(tBuffer,StrideSize,1,pFile);
        for(int j = 0; j < oInfoHeader.biWidth; ++j)
        {
            aImage.GetRasterData()[(aImage.GetHeight()-i-1)*aImage.GetWidth()+j] 
                = ColorRGB(
                    tBuffer[3*j+2]/255.0f,
                    tBuffer[3*j+1]/255.0f, 
                    tBuffer[3*j]/255.0f);
        }
    }

    fclose(pFile);
    delete[] tBuffer;
}
