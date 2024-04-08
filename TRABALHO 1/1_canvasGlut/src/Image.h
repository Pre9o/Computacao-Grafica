//*********************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
// Autor: Cesar Tadeu Pozzer
//        pozzer@inf.ufsm.br
//
//  Referencia:  http://astronomy.swin.edu.au/~pbourke/dataformats/bmp/
//  Versao 09/2010
//
//**********************************************************

#ifndef ___BMP__H___
#define ___BMP__H___

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <vector>

#define HEADER_SIZE      14 //sizeof(HEADER) vai dar 16 devido ao alinhamento de bytes
#define INFOHEADER_SIZE  40 //sizeof(INFOHEADER) da 40 e esta correto.
#define uchar unsigned char

typedef struct {
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data em bytes*/
} HEADER;


typedef struct {
   unsigned int size;                  /* Header size in bytes      */
   int width, height;                  /* Width and height of image */
   unsigned short int planes;          /* Number of colour planes   */
   unsigned short int bits;            /* Bits per pixel            */
   unsigned int compression;           /* Compression type          */
   unsigned int imagesize;             /* Image size in bytes       */
   int xresolution, yresolution;       /* Pixels per meter          */
   unsigned int ncolours;              /* Number of colours         */
   unsigned int impcolours;            /* Important colours         */
} INFOHEADER;


class Bmp
{
private:
   int width, height, imagesize, bytesPerLine, bits;
   unsigned char *data;

   HEADER     header;
   INFOHEADER info;

   void load(const char *fileName);
   unsigned char* originalData;

public:
   Bmp(const char *fileName);
   uchar* getImage();
   void   setImage(uchar* img);
   int    getWidth(void);
   void   setWidth(int w);
   int    getHeight(void);
   void   setHeight(int h);
   void   convertBGRtoRGB(void);
   void   adjustBrightness(int brightness);
   void   adjustContrast(float contrast);
   int    contains(int x, int y);
   void   flipVertical(void);
   void   flipHorizontal(void);
   void   image_R(void);
   void   image_G(void);
   void   image_B(void);
   void   image_Gray(void);
   int    x_start, y_start, x_end, y_end;


   Bmp(const Bmp& other) {
      width = other.width;
      height = other.height;
      x_start = other.x_start;
      y_start = other.y_start;
      x_end = other.x_end;
      y_end = other.y_end;
      bits = other.bits;
      bytesPerLine = other.bytesPerLine;

      
      // Para o campo 'data', você precisa alocar nova memória e copiar os dados
      data = new unsigned char[3 * other.width * other.height];
      memcpy(data, other.data, 3 * other.width * other.height);
   }
};

#endif