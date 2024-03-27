#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bmp.h"

#define MAX_TEXTURES 3

#define RECT_SIZE 10
#define TEXT_COORD 2

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;


int opcao  = 50; //variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Bmp *image_1 = NULL;
Bmp *image_2 = NULL;

void DrawMouseScreenCoords(){
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

void DrawImage(Bmp *image){
   for(int i = 0; i < image_1->getHeight(); i++)
   {
      for(int j = 0; j < image_1->getWidth(); j++)
      {
         
      }
   }
}


void render()
{
   CV::translate(250, 250);

   for(int i = 0; i < image_1->getHeight(); i++)
   {
      for(int j = 0; j < image_1->getWidth(); j++)
      {
         int pos = i*image_1->getWidth()*3 + j*3;
         int pos_2 = i*image_2->getWidth()*3 + j*3;
         CV::color(image_1->getImage()[pos]/255.0, image_1->getImage()[pos+1]/255.0, image_1->getImage()[pos+2]/255.0);
         CV::rectFill(j, i, j + 1, i + 1);

         CV::color(image_2->getImage()[pos_2]/255.0, image_2->getImage()[pos_2+1]/255.0, image_2->getImage()[pos_2+2]/255.0);
         CV::rectFill(j + 200, i, j + 201, i + 1);
      }
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
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}


int main()
{
   CV::init(screenHeight, screenWidth, "Canvas2D");

   image_1 = new Bmp(".\\images\\pinguim.bmp");
   image_1->convertBGRtoRGB();

   image_2 = new Bmp(".\\images\\bmp_24.bmp");
   image_2->convertBGRtoRGB();

   CV::run();
   
   return 0;
}
