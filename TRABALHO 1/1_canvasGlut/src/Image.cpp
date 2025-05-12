/*
  A classe Bmp é usada para manipular imagens no formato BMP. Ela fornece funcionalidades para carregar uma imagem BMP de um arquivo,
  obter e definir os dados da imagem, obter e definir a largura e a altura da imagem, converter a imagem de BGR para RGB,
  ajustar o brilho e o contraste da imagem, verificar se um ponto está contido na imagem, inverter a imagem vertical e horizontalmente,
  alterar a imagem para mostrar apenas o canal vermelho, verde, azul ou cinza, e carregar uma imagem BMP do arquivo especificado.
 */

#include "Image.h"
#include <string.h>
#include <algorithm>
#include "gl_canvas2d.h"
#include <iostream>

// Construtor da classe Bmp. Carrega uma imagem BMP do arquivo especificado.
Bmp::Bmp(const char *fileName)
{
   width = height = 0;
   data = NULL;
   if( fileName != NULL && strlen(fileName) > 0 )
   {
      load(fileName);
   }
   else
   {
      printf("Error: Invalid BMP filename");
   }
}

// Retorna um ponteiro para os dados da imagem.
uchar* Bmp::getImage()
{
return data;
}

// Define os dados da imagem.
void Bmp::setImage(uchar* img)
{
  this->data = img;
}

// Retorna a largura da imagem.
int Bmp::getWidth(void)
{
return width;
}

// Define a largura da imagem.
void Bmp::setWidth(int w)
{
   this->width = w;
}

// Retorna a altura da imagem.
int Bmp::getHeight(void)
{
return height;
}

// Define a altura da imagem.
void Bmp::setHeight(int h)
{
   this->height = h;
}

// Converte a imagem de BGR para RGB.
void Bmp::convertBGRtoRGB()
{
unsigned char tmp;
if( data != NULL )
{
   for(int y=0; y<height; y++)
   for(int x=0; x<width*3; x+=3)
   {
      int pos = y * width * 3 + x;
      tmp = data[pos];
      data[pos] = data[pos+2];
      data[pos+2] = tmp;
   }
}
}

// Ajusta o brilho da imagem.
void Bmp::adjustBrightness(int brightness) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();

   for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++){
         int pos = i * width * 3 + j * 3;
         img[pos] = std::min(255, std::max(0, img[pos] + brightness));
         img[pos + 1] = std::min(255, std::max(0, img[pos + 1] + brightness));
         img[pos + 2] = std::min(255, std::max(0, img[pos + 2] + brightness));
      }
   }
}

// Ajusta o contraste da imagem.
void Bmp::adjustContrast(float contrast) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   float factor = (259 * (contrast + 255)) / (255 * (259 - contrast));

   for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++){
         int pos = i * width * 3 + j * 3;
         img[pos] = std::min(255.0f, std::max(0.0f, factor * (img[pos] - 128) + 128));
         img[pos + 1] = std::min(255.0f, std::max(0.0f, factor * (img[pos + 1] - 128) + 128));
         img[pos + 2] = std::min(255.0f, std::max(0.0f, factor * (img[pos + 2] - 128) + 128));
      }
   }
}

// Verifica se um ponto está contido na imagem.
int Bmp::contains(int x, int y)
{
   if( x >= x_start && x <= x_end && y >= y_start && y <= y_end )
   {
      return 1;
   }
   return 0;
}

// Inverte a imagem verticalmente.
void Bmp::flipVertical() {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   for (int i = 0; i < height; i++) {
      std::reverse(img + i * width * 3, img + (i+1) * width * 3);
   }
   convertBGRtoRGB();
}

// Inverte a imagem horizontalmente.
void Bmp::flipHorizontal() {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   for (int i = 0; i < width; i++) {
      for (int j = 0; j < height / 2; j++) {
            std::swap(img[(j * width + i) * 3], img[((height - j - 1) * width + i) * 3]);
            std::swap(img[(j * width + i) * 3 + 1], img[((height - j - 1) * width + i) * 3 + 1]);
            std::swap(img[(j * width + i) * 3 + 2], img[((height - j - 1) * width + i) * 3 + 2]);
      }
   }
}

// Altera a imagem para mostrar apenas o canal vermelho.
void Bmp::image_R(void) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
         int pos = i * width * 3 + j * 3;
         img[pos + 1] = 0;
         img[pos + 2] = 0;
      }
   }
}

// Altera a imagem para mostrar apenas o canal verde.
void Bmp::image_G(void) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
         int pos = i * width * 3 + j * 3;
         img[pos] = 0;
         img[pos + 2] = 0;
      }
   }
}

// Altera a imagem para mostrar apenas o canal azul.
void Bmp::image_B(void) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();

   for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++){
         int pos = i * width * 3 + j * 3;
         img[pos] = 0;
         img[pos + 1] = 0;
      }
   }
}

// Altera a imagem para mostrar apenas o canal cinza.
void Bmp::image_Gray(void) {
   unsigned char* img = this->getImage();
   int width = this->getWidth();
   int height = this->getHeight();
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
         int pos = i * width * 3 + j * 3;
         float gray = 0.299 * img[pos] + 0.587 * img[pos + 1] + 0.114 * img[pos + 2];
         img[pos] = gray;
         img[pos + 1] = gray;
         img[pos + 2] = gray;
      }
   }
}

// Aplica um blur gaussiano na imagem.
void Bmp::blurGaussian(int quantidadeBlur) {
      if (quantidadeBlur > 10) {
         quantidadeBlur = 10;
      }
      if (quantidadeBlur < 1) {
         quantidadeBlur = 1;
      }

   int width = this->getOriginalWidth();
   int height = this->getOriginalHeight();
   unsigned char* src = this->getOriginalImage();
   unsigned char* dst = new unsigned char[width * height * 3];

   for (int i = 0; i < quantidadeBlur; i++) {
      // Kernel Gaussiano 3x3 fixo (sigma ~1.0)
      float kernel[3][3] = {
         {1/16.f, 2/16.f, 1/16.f},
         {2/16.f, 4/16.f, 2/16.f},
         {1/16.f, 2/16.f, 1/16.f}
      };

      for (int y = 1; y < height-1; y++) {
         for (int x = 1; x < width-1; x++) {
               for (int c = 0; c < 3; c++) {
                  float sum = 0.0f;
                  for (int ky = -1; ky <= 1; ky++) {
                     for (int kx = -1; kx <= 1; kx++) {
                           int px = x + kx;
                           int py = y + ky;
                           int pos = (py * width + px) * 3 + c;
                           sum += src[pos] * kernel[ky+1][kx+1];
                     }
                  }
                  dst[(y * width + x) * 3 + c] = (unsigned char)sum;
               }
         }
      }

      // Copia bordas sem alteração
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
               if (y == 0 || y == height-1 || x == 0 || x == width-1) {
                  for (int c = 0; c < 3; c++) {
                     dst[(y * width + x) * 3 + c] = src[(y * width + x) * 3 + c];
                  }
               }
         }
      }

      // Atualiza a imagem
      delete[] this->data;
      this->data = dst;

      width = this->getWidth();
      height = this->getHeight();
      src = this->getImage();
      dst = new unsigned char[width * height * 3];
   }
}

// Carrega uma imagem BMP do arquivo especificado.
void Bmp::load(const char *fileName)
{

FILE *fp = fopen(fileName, "rb");
if( fp == NULL )
{
   printf("\nErro ao abrir arquivo %s para leitura", fileName);
   return;
}

printf("\n\nCarregando arquivo %s", fileName);

fread(&header.type,      sizeof(unsigned short int), 1, fp);
fread(&header.size,      sizeof(unsigned int),       1, fp);
fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
fread(&header.offset,    sizeof(unsigned int),       1, fp);

fread(&info.size,        sizeof(unsigned int),       1, fp);
fread(&info.width,       sizeof(int),                1, fp);
fread(&info.height,      sizeof(int),                1, fp);
fread(&info.planes,      sizeof(unsigned short int), 1, fp);
fread(&info.bits,        sizeof(unsigned short int), 1, fp);
fread(&info.compression, sizeof(unsigned int),       1, fp);
fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
fread(&info.xresolution, sizeof(int),                1, fp);
fread(&info.yresolution, sizeof(int),                1, fp);
fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

width  = info.width;
height = info.height;
bits   = info.bits;

bytesPerLine = (3 * (width + 1) / 4) * 4;
imagesize    = bytesPerLine * height;
int delta    = bytesPerLine - (3 * width);

printf("\nImagem: %dx%d - Bits: %d", width, height, bits);
printf("\nbytesPerLine: %d", bytesPerLine);
printf("\nbytesPerLine: %d", width * 3);
printf("\ndelta: %d", delta);
printf("\nimagesize: %d %d", imagesize, info.imagesize);

if( header.type != 19778 )
{
   printf("\nError: Arquivo BMP invalido");
   getchar();
   exit(0);
}

if( width*height*3 != imagesize )
{
   printf("\nWarning: Arquivo BMP nao tem largura multipla de 4");
}

if( info.compression != 0 )
{
   printf("\nError: Formato BMP comprimido nao suportado");
   getchar();
   return;
}
if( bits != 24 )
{
   printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
   getchar();
   return;
}

if( info.planes != 1 )
{
   printf("\nError: Numero de Planes nao suportado: %d", info.planes);
   getchar();
   return;
}

data = new unsigned char[3 * width * height];
for(int i = 0; i < height; i++){
   int pos = i * bytesPerLine;
   fseek(fp, header.offset + pos, SEEK_SET);
   fread(data + width * 3 * i, sizeof(unsigned char), width * 3, fp);
}

fclose(fp);
}