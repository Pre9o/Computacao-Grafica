#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "gl_canvas2d.h"
#include "Bmp.h"
#include "Botao.h"

#define MAX_IMAGES 3

#define RECT_SIZE 10
#define TEXT_COORD 2

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1520, screenHeight = 1080;


int opcao  = 50; //variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int clicando = 0;

std::vector<Bmp*> images;
std::vector<Botao*> botoes;

Bmp* draggingImage = NULL;
Bmp* selectedImage = NULL;

void ConstruirBotoes(){
   if(botoes.size() == 0){
      botoes.push_back(new Botao(10, 10, 100, 30, "Flip Vertical"));
      botoes.push_back(new Botao(10, 50, 100, 30, "Flip Horizontal"));
      botoes.push_back(new Botao(10, 90, 100, 30, "Flip Diagonal Principal"));
      botoes.push_back(new Botao(10, 130, 100, 30, "Flip Diagonal Secundaria"));
   }

   for (Botao* botao : botoes) {
      botao->Render();
   }
}

void DesenharImagemSelecionadaRGB(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, 0, 0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);

         CV::color(0, image->getImage()[pos+1]/255.0, 0);
         CV::rectFill(j + image->x_start, i + image->y_start + image->getHeight(), j + image->x_start + 1, i + image->y_start + image->getHeight() + 1);

         CV::color(0, 0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + image->x_start, i + image->y_start + 2*image->getHeight(), j + image->x_start + 1, i + image->y_start + 2*image->getHeight() + 1);
      }
   }
}

void DesenharImagemSelecionadaGrayscale(Bmp* image){
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

void ManipularVetorImagem(Bmp* image){
   auto it = std::find(images.begin(), images.end(), image);
   if (it != images.end()) {
      std::rotate(images.rbegin(), std::make_reverse_iterator(it+1), images.rend());
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

void DrawMouseScreenCoords(){
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

void InicializarParametros(Bmp *image, int offset){
   image->x_start = offset;
   image->y_start = 0;
   image->x_end = offset + image->getWidth();
   image->y_end = image->getHeight();
}


void DrawImage(Bmp *image){
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


void LoadImages(){
   images.push_back(new Bmp(".\\images\\pinguim.bmp"));
   images.push_back(new Bmp(".\\images\\bmp_24.bmp"));
   images.push_back(new Bmp(".\\images\\snail.bmp"));

   for(int i = 0; i < images.size(); i++){
      InicializarParametros(images[i], i * 300);
      images[i]->convertBGRtoRGB();
   }
}

void render()
{
   CV::translate(500, 500);

   DrawImage(images[0]);
   DrawImage(images[1]);
   DrawImage(images[2]);

   ConstruirBotoes();


   if(selectedImage != NULL){
      DesenharMoldura(selectedImage);
      //DesenharImagemSelecionadaGrayscale(selectedImage);
      //DesenharImagemSelecionadaRGB(selectedImage);
   }

   Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 'v':
         if (selectedImage != NULL) {
            printf("\nFlip vertical");
            selectedImage->flipVertical();
         }
         break;
      case 'h':
         if (selectedImage != NULL) {
            printf("\nFlip horizontal");
            selectedImage->flipHorizontal();
         }
         break;
      case 'p':
         if (selectedImage != NULL) {
            printf("\nFlip diagonal principal");
            selectedImage->flipDiagonalPrincipal();
         }
         break;
      case 's':
         if (selectedImage != NULL) {
            printf("\nFlip diagonal secundaria");
            selectedImage->flipDiagonalSecundaria();
         }
         break;
      case 27:
         exit(0);
      break;
   }
}
//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   x -= 500;
   y -= 500;

   mouseX = x;
   mouseY = y;

   if(clicando){
      ArrastarImagem(draggingImage, x, y);
   }


   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);

   if (button == 0) {
      for (Bmp* image : images) {
            if (image->contains(x, y && draggingImage == NULL)) {
               selectedImage = image;
               printf("\nImagem selecionada: %d", selectedImage->getWidth());
               ManipularVetorImagem(image);
            }
            else {
               selectedImage = NULL;
            }
      }
      if(state == 1) {
      // Parar o arrasto
      draggingImage = nullptr;
      clicando = 0;
      }
      else if (state == 0) {
         // Iniciar o arrasto
         for (Bmp* image : images) {
            if (image->contains(x, y)) {
               selectedImage = image;
               draggingImage = image;
               clicando = 1;
               ManipularVetorImagem(image);
            }
         }
      }
   }
}


int main()
{
   CV::init(screenHeight, screenWidth, "Canvas2D");
   LoadImages();
   CV::run();
   
   return 0;
}
