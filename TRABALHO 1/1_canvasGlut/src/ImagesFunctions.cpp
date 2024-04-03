#include <vector>
#include <algorithm>
#include "gl_canvas2d.h"
#include "Bmp.h"

void DesenharImagemSelecionadaRed(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, 0, 0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void DesenharImagemSelecionadaGreen(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(0, image->getImage()[pos+1]/255.0, 0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void DesenharImagemSelecionadaBlue(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(0, 0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void DesenharImagemSelecionadaGray(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos]/255.0, image->getImage()[pos]/255.0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void ManipularVetorImagem(std::vector<Bmp*>& images, Bmp* &image){
   // mover a image para a última posição do vetor
   std::vector<Bmp*>::iterator it = std::find(images.begin(), images.end(), image);
   if(it != images.end()){
      std::iter_swap(it, images.end() - 1);
   }
}

void ArrastarImagem(Bmp* image, int x, int y) {
   image->x_start = x - image->getWidth()/2;
   image->y_start = y - image->getHeight()/2;
   image->x_end = x + image->getWidth()/2;
   image->y_end = y + image->getHeight()/2;
}

void DesenharMoldura(Bmp* image) {
   CV::color(0, 0, 0);
   CV::rect(image->x_start, image->y_start, image->x_end, image->y_end);
}

void DrawImage(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void InicializarParametros(Bmp* image, int offset){
   image->x_start = offset;
   image->y_start = 0;
   image->x_end = offset + image->getWidth();
   image->y_end = image->getHeight();
   printf("inicializado");
}


void LoadImages(std::vector<Bmp*> &images){
   images.push_back(new Bmp(".\\images\\pinguim.bmp"));
   images.push_back(new Bmp(".\\images\\bmp_24.bmp"));
   images.push_back(new Bmp(".\\images\\snail.bmp"));

   for(int i = 0; i < images.size(); i++){
      InicializarParametros(images[i], i * 300);
      images[i]->convertBGRtoRGB();
   }
}