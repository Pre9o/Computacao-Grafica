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

Bmp *bmp = NULL;
unsigned char *data = NULL;
GLuint textureID[MAX_TEXTURES];

float ang=0;


void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}


void render()
{
   CV::text(20,500,"Programa Demo Canvas2D");

   CV::translate(250, 250);

   for(int i = 0; i < bmp->getHeight(); i++)
   {
      for(int j = 0; j < bmp->getWidth(); j++)
      {
         int pos = i*bmp->getWidth()*3 + j*3;
         CV::color(bmp->getImage()[pos]/255.0, bmp->getImage()[pos+1]/255.0, bmp->getImage()[pos+2]/255.0);
         CV::rectFill(j, i, j + 1, i + 1);
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

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}


int main()
{
   CV::init(screenHeight, screenWidth, "Canvas2D");

   bmp = new Bmp(".\\images\\pinguim.bmp");
   bmp->convertBGRtoRGB();


   CV::run();
   
   return 0;
}
