/**
*   Programa para ilustrar:
*   - Uso da gluLookAt
*   - Pilha de transformacoes
*   - Projecao pespectiva
*   - Interacao com o mouse/teclado
*   - cullFace, frontFace

*   Autor: Cesar Tadeu Pozzer
*   UFSM - 15/06/2007
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glut.h>


#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "Vector3.h"

#include <time.h>

#define SCREEN_X 500
#define SCREEN_Y 500

int polygonMode = 1;

float rx = 0, rz = 0; //parametros de rotacao do objeto.

float abertura = 20.0;
float znear  = 1;
float zfar   = 23;
float aspect = 1;

Vector3 heightMap[100][100];


//gerar um mapa de altura com valores aleatorios
void generateHeightMap()
{
   int i, j;
   for(i=0; i<100; i++)
   {
      for(j=0; j<100; j++)
      {
         heightMap[i][j].set(float(rand()%100), float(rand()%100), float(rand()%100));
      }
   }
}


void drawHeightMap()
{
   int i, j;
   for(i=0; i<100-1; i++)
   {
      for(j=0; j<100-1; j++)
      {

         glBegin(GL_TRIANGLE_STRIP);
         printf("x=%f y=%f z=%f\n", heightMap[i][j].x, heightMap[i][j].y, heightMap[i][j].z);
            glColor3f(1, 1, heightMap[i][j].z);
            glVertex3f(i, heightMap[i][j].x, j);

            glColor3f(1, 1, heightMap[i][j].z);
            glVertex3f(i+1, heightMap[i+1][j].x, j);

            glColor3f(1, 1, heightMap[i][j].z);
            glVertex3f(i, heightMap[i][j+1].x, j+1);

            glColor3f(1, 1, heightMap[i][j].z);
            glVertex3f(i+1, heightMap[i+1][j+1].x, j+1);
         glEnd();
      }
   }
}

void init()
{

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glEnable(GL_DEPTH_TEST);
}

//imprime as matrizes da pilha do opengl
void printMatrix(GLenum pname)
{
   GLfloat m[16]; //usado para leitura das matrizes
   glGetFloatv (pname, m);
   int l, c;
   for(l=0; l<4; l++)
   {
      for(c=0; c<4; c++)
      {
         int pos = l * 4 + c;
      }
   }
}


////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   //glOrtho(-1,1,-1,1,1,-1);

   printMatrix(GL_PROJECTION_MATRIX);

   gluPerspective(abertura, aspect, znear, zfar);


   printMatrix(GL_MODELVIEW_MATRIX);

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity( );
   gluLookAt(0, 0, 15,  //from. Posicao onde a camera esta posicionada
             0, 0, 0,  //to. Posicao absoluta onde a camera esta vendo
             0, 1, 0); //up. Vetor Up.

   glRotatef ((GLfloat) rx, 0.0f, 1.0f, 0.0f);
   glRotatef ((GLfloat) rz, 1.0f, 0.0f, 0.0f);

   //todos os objetos estao definidos na origem do sistema global fixo
   //e sao transladados para a posicao destino.
   glColor3f(1, 1, 1);
   glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 1);
      glVertex3f(1, 0, 1);
      glVertex3f(1, 0, -1);
      glVertex3f(-1, 0, -1);
   glEnd();

   //bule 1
   glPushMatrix();
      glColor3f(1, 0, 1);
      glTranslated(-0.5, 0.15, 0.5);
      glutWireTeapot(0.2);
   glPopMatrix();

   //bule 2
   glPushMatrix();
      glColor3f(0, 1, 0);
      glTranslated(0.5, 0.15, -0.5);
      glRotated(90, 0, 1, 0);
      glutWireTeapot(0.2);
   glPopMatrix();

   //bule 3
   glColor3f(0, 0, 1);
   //glutSolidIcosahedron();
   glutSolidTeapot(1.2);

   //desenha o mapa de altura
   drawHeightMap();


   glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   key = tolower(key);
   switch(key)
   {
      case 27:
	     exit(0);
	  break;

      case 'w': //wireframe
         if(polygonMode==1)
		 {
		    polygonMode=0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	     }
		 else
		 {
		    polygonMode=1;
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		 }
		break;
      case '+':
           abertura+=1;

   }
}

void MotionFunc(int x, int y)
{
//   float dx, dy;
   rx = x;
   rz = y;
}

void MouseFunc(int botao, int estado, int x, int y)
{
}

////////////////////////////////////////////////////////////////////////////////////////
int main ()
{
   int  argc=0;
   //char *argv[1] = {"teste"};
   glutInit(&argc, NULL);

   srand(time(NULL));

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   //glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (450, 10);
   /* create window */
   glutCreateWindow ("GluLookAt Demo");
   init();
   printf("Digite W para mudar o modo de visualizacao: wireframe/fill");

   glutDisplayFunc(display);
   glutMotionFunc(MotionFunc);
   glutMouseFunc(MouseFunc);
   glutIdleFunc(display);
   glutKeyboardFunc(keyboard);

   glutMainLoop();
   return 0;
}
