#include <vector>
#include <algorithm>
#include "gl_canvas2d.h"
#include "Image.h"
#include <numeric>

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
}


void LoadImages(std::vector<Bmp*> &images, const char* path){
   Bmp* image_loaded = new Bmp(path);
   InicializarParametros(image_loaded, images.size() * 100);
   image_loaded->convertBGRtoRGB();
   images.push_back(image_loaded);
}

void Draw4thImage(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + 1100, i + 200, j + 1101, i + 201);
      }
   }
}

void DesenharHistogramaRed(Bmp* image){
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos]]++;
      }
   }

   int max = *std::max_element(histogram, histogram + 256);
   int min = *std::min_element(histogram, histogram + 256);
   float avg = std::accumulate(histogram, histogram + 256, 0.0) / 256;

   for(int i = 0; i < 256; i++){
      CV::color(0, 0, 0);
      float height = std::log(1 + histogram[i]) / std::log(1 + max) * 300;
      CV::line(i + 900, 300 - height, i + 900, 300);
   }
   CV::color(1, 0, 1);
   CV::line(900, 300 - avg / max * 300, 1156, 300 - avg / max * 300);
}