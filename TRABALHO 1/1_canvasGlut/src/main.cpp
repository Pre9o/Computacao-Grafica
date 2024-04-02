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
#include "ImagesFunctions.h"

#define MAX_IMAGES 3

#define RECT_SIZE 10
#define TEXT_COORD 2

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1520, screenHeight = 1080;

bool grayscale = false;

int opcao  = 50; //variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int clicando = 0;

std::vector<Bmp*> images;
std::vector<Botao*> botoes;

Bmp* draggingImage = NULL;
Bmp* selectedImage = NULL;

void ConstruirBotoes(){
   if(botoes.size() == 0){
      botoes.push_back(new Botao(1250, -400, 150, 50, "Flip Vertical", 1, 1, 0, [](){
         if (selectedImage != NULL) {
            printf("\nFlip vertical");
            selectedImage->flipVertical();
         }
      }));
      botoes.push_back(new Botao(1075, -400, 150, 50, "Flip Horizontal", 0, 1, 0, [](){
         if (selectedImage != NULL) {
            printf("\nFlip horizontal");
            selectedImage->flipHorizontal();
         }
      }));
      botoes.push_back(new Botao(1250, -325, 150, 50, "Grayscale", 0.5, 0.5, 0.5, [](){
         if (selectedImage != NULL) {
            printf("\nGrayscale");
            grayscale = !grayscale;
         }
      }));
      botoes.push_back(new Botao(1075, -325, 150, 50, "Blue", 0, 0, 1, [](){
         if (selectedImage != NULL) {
            printf("\nBlue");
            DesenharImagemSelecionadaBlue(selectedImage);
         }
      }));
   }
}


void DrawMouseScreenCoords(){
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}


void render()
{
   CV::translate(500, 500);
   DrawImage(images[0]);
   DrawImage(images[1]);
   DrawImage(images[2]);

   for (Botao* botao : botoes) {
      botao->Render();
   }

   if(grayscale){
      DesenharImagemSelecionadaGray(selectedImage);
   }


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
            if (image->contains(x, y && draggingImage == nullptr)) {
               selectedImage = image;
               printf("\nImagem selecionada: %d", selectedImage->getWidth());
               ManipularVetorImagem(images, image);
            }
            else if(!image->contains(x, y) && draggingImage == nullptr){
               //selectedImage = nullptr;
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
               ManipularVetorImagem(images, image);
            }
         }

         for (Botao* botao : botoes) {
            if(botao->Colidiu(x, y)){
               botao->onClick();
            }
         }
      }
   }
}


int main()
{
   CV::init(screenHeight, screenWidth, "Canvas2D");
   LoadImages(images);
   ConstruirBotoes();
   CV::run();
   
   return 0;
}