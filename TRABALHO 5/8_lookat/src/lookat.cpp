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

#define SCREEN_X 1280
#define SCREEN_Y 720

int polygonMode = 1;

float rx = 0, rz = 0; //parametros de rotacao do objeto.

float abertura = 59.0;
float znear  = 0.1;
float zfar   = 5000;
float aspect = 16.0/9.0;

Vector3 cameraPos(1, 1, 1);
Vector3 cameraDir(0, 0, 0.1);
Vector3 up(0, 1, 0);

Vector3 heightMap[100][100];

Vector3 cilindros[5][5];

Vector3 controlPoints[4][4] = {
        {{0, 0, 0}, {1, 0, 0}, {2, -1, 0}, {3, 0, 0}},
        {{0, 0, 0}, {1, 1, 0}, {2, -0, 0}, {3, 0, 0}},
        {{0, 0, 2}, {1, 0, 2}, {2, 0, 2}, {3, 0, 2}},
        {{0, 0, 3}, {1, 1, 3}, {2, -1, 3}, {3, 0, 3}}
    };

void generateRandomControlPoints() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            controlPoints[i][j] = Vector3((rand() % 100) / 10.0, (rand() % 100) / 10.0, (rand() % 100) / 10.0);
        }
    }
}

// Função para calcular um ponto em uma curva de Bézier
Vector3 bezierPoint(Vector3* points, int numPoints, float t) {
      // Se houver apenas um ponto, retorne-o
      if (numPoints == 1) {
         return points[0];
      }
      // Aloca um vetor temporário para armazenar os pontos intermediários
      Vector3* tempPoints = new Vector3[numPoints - 1];
      // Calcula os pontos intermediários para a direção t
      for (int i = 0; i < numPoints - 1; i++) {
         tempPoints[i] = points[i] * (1 - t) + points[i + 1] * t;
      }
      // Chama a função recursivamente para os pontos intermediários
      Vector3 result = bezierPoint(tempPoints, numPoints - 1, t);
      // Libera a memória alocada para o vetor temporário
      delete[] tempPoints;
      // Retorna o ponto final na curva
      return result;
}


// Função para calcular um ponto em uma superfície de Bézier
Vector3 bezierSurfacePoint(Vector3 points[4][4], float u, float v) {
    Vector3 tempPoints[4];
    for (int i = 0; i < 4; i++) {
        // Calcula os pontos intermediários para a direção u
        tempPoints[i] = bezierPoint(points[i], 4, u);
    }
    // Calcula o ponto final na superfície usando os pontos intermediários e o parâmetro v
    return bezierPoint(tempPoints, 4, v);
}

// Modificar a função generateHeightMap para usar a superfície de Bézier
void generateHeightMap() {
    // Usar os pontos de controle para gerar o mapa de altura
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            float u = i / 99.0; // Normaliza i para o intervalo [0, 1]
            float v = j / 99.0; // Normaliza j para o intervalo [0, 1]
            heightMap[i][j] = bezierSurfacePoint(controlPoints, u, v);
        }
    }

}

// Função para calcular a altura de um ponto no terreno
Vector3 calculateHeight(float x, float z) {
      // Calcula a posição do ponto no intervalo [0, 1]

      float u = x/3;
      float v = z/3;
      // Calcula a altura do ponto na superfície de Bézier
      return bezierSurfacePoint(controlPoints, u, v);
   }


void generateCylinderOnTerrain(){
   for(int i=0; i<5; i+=1)
   {
      for(int j=0; j<5; j+=1)
      {
         float x = rand() % 290/100;
         float z = rand() % 290/100;
         printf("x: %f, z: %f\n", x, z);
         //float y = calculateHeight(x, z);
         Vector3 teste = calculateHeight(x, z);
         //printf("x: %f, y: %f, z: %f\n", x, y, z);
         printf("x: %f, y: %f, z: %f\n", teste.x, teste.y, teste.z);

         cilindros[i][j] = Vector3(teste.x, teste.y, teste.z);
      }

  }
}

void normalizePoint(float &x, float &y, float &z) {
    float length = sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

void drawTriangle(float *a, float *b, float *c) {
    glBegin(GL_TRIANGLES);
        glVertex3fv(a);
        glVertex3fv(b);
        glVertex3fv(c);
    glEnd();
}

void divideFace(float *a, float *b, float *c, int depth) {
    if (depth == 0) {
        drawTriangle(a, b, c);
    } else {
        float ab[3], ac[3], bc[3];

        for (int i = 0; i < 3; i++) {
            ab[i] = (a[i] + b[i]) / 2;
            ac[i] = (a[i] + c[i]) / 2;
            bc[i] = (b[i] + c[i]) / 2;
        }
        normalizePoint(ab[0], ab[1], ab[2]);
        normalizePoint(ac[0], ac[1], ac[2]);
        normalizePoint(bc[0], bc[1], bc[2]);

        divideFace(a, ab, ac, depth - 1);
        divideFace(ab, b, bc, depth - 1);
        divideFace(ac, bc, c, depth - 1);
        divideFace(ab, bc, ac, depth - 1);
    }
}

void drawSphereFromTetrahedron(int depth, float scale) {
    float v[4][3] = {
        {0, 0, 1 * scale},
        {0, 2 * sqrt(2.0f / 3.0f) * scale, -1.0f / 3 * scale},
        {-sqrt(2.0f / 3.0f) * scale, -sqrt(2.0f / 3.0f) * scale, -1.0f / 3 * scale},
        {sqrt(2.0f / 3.0f) * scale, -sqrt(2.0f / 3.0f) * scale, -1.0f / 3 * scale}
    };

    divideFace(v[0], v[1], v[2], depth);
    divideFace(v[0], v[2], v[3], depth);
    divideFace(v[0], v[3], v[1], depth);
    divideFace(v[1], v[3], v[2], depth);
}

void drawTetrahedron(float edgeSize) {
    // Altura do tetraedro equilátero a partir do comprimento da aresta
    float height = sqrt(2.0f / 3.0f) * edgeSize;
    float sideHeight = sqrt(3.0f) / 2 * edgeSize; // Altura de um lado do triângulo equilátero

    // Pontos base do tetraedro
    float points[4][3] = {
        { 0, height, 0 }, // Topo
        { -edgeSize / 2, 0, -sideHeight / 3 }, // Base esquerda
        { edgeSize / 2, 0, -sideHeight / 3 }, // Base direita
        { 0, 0, 2.0f / 3.0f * sideHeight } // Base frente
    };

    glBegin(GL_TRIANGLES);
        // Base
        glVertex3fv(points[1]);
        glVertex3fv(points[2]);
        glVertex3fv(points[3]);

        // Lados
        glVertex3fv(points[0]);
        glVertex3fv(points[2]);
        glVertex3fv(points[1]);

        glVertex3fv(points[0]);
        glVertex3fv(points[3]);
        glVertex3fv(points[2]);

        glVertex3fv(points[0]);
        glVertex3fv(points[1]);
        glVertex3fv(points[3]);
    glEnd();
}

void drawCylindersOnTerrain(){
   for(int i=0; i<5; i+=1)
   {
      for(int j=0; j<5; j+=1)
      {
         GLUquadric *quadric = gluNewQuadric();

         glPushMatrix();
            glColor3f(0.545098, 0.270588, 0.07450980); // Cor do tronco
            glTranslatef(cilindros[i][j].x, cilindros[i][j].y, cilindros[i][j].z);
            glRotatef(270, 1, 0, 0); // Rotaciona para que o cilindro fique de pé
            gluCylinder(quadric, 0.02, 0.02, 0.2, 10, 10); // Desenha o cilindro
         glPopMatrix();

         // Desenha o tetraedro no topo do cilindro
         glPushMatrix();
            glColor3f(0.0, 1.0, 0.0); // Cor da copa da árvore (verde)
            // Ajusta a posição para o topo do cilindro. Ajuste o 0.2 conforme a altura do seu cilindro
            glTranslatef(cilindros[i][j].x, cilindros[i][j].y + 0.2, cilindros[i][j].z);
            drawTetrahedron(0.1); // Desenha o tetraedro
         glPopMatrix();
      }
   }
}

void drawHeightMap()
{
   int i, j;

   for(i = 0; i < 100 - 1; i++)
   {
      for(j = 0; j < 100 - 1; j++)
      {
         glBegin(GL_TRIANGLE_STRIP);
            glColor3f(0, 0.25, 0);
            glVertex3f(heightMap[i][j].x, heightMap[i][j].y, heightMap[i][j].z);

            glColor3f(0, 0.25, 0);
            glVertex3f(heightMap[i][j+1].x, heightMap[i][j+1].y, heightMap[i][j+1].z);

            glColor3f(0, 0.25, 0);
            glVertex3f(heightMap[i+1][j].x, heightMap[i+1][j].y, heightMap[i+1][j].z);

            glColor3f(0, 0.25, 0);
            glVertex3f(heightMap[i+1][j+1].x, heightMap[i+1][j+1].y, heightMap[i+1][j+1].z);


         glEnd();
      }
   }
      drawCylindersOnTerrain();

   //drawLinesOnControlPoints();
}




Vector3 rotateX(Vector3 v, float angle) {
    float rad = angle * 3.14159265 / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA);
}

Vector3 rotateY(Vector3 v, float angle) {
    float rad = angle * 3.14159265 / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA);
}

Vector3 rotateZ(Vector3 v, float angle) {
    float rad = angle * 3.14159265 / 180;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vector3(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z);
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



////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   //glOrtho(-1,1,-1,1,1,-1);


   gluPerspective(abertura, aspect, znear, zfar);



   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity( );
   cameraDir.normalize();
   gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraDir.x, cameraPos.y + cameraDir.y, cameraPos.z + cameraDir.z, up.x, up.y, up.z);
   //glRotatef ((GLfloat) rx, 0.0f, 1.0f, 0.0f);
   //glRotatef ((GLfloat) rz, 1.0f, 0.0f, 0.0f);

   //todos os objetos estao definidos na origem do sistema global fixo
   //e sao transladados para a posicao destino.
   /*
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
   */

   //desenha o mapa de altura
   drawHeightMap();


   glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   key = tolower(key);

   Vector3 right = cameraDir.cross(up);
   right.normalize();


   switch(key)
   {
      case 27:
	     exit(0);
	  break;

      case 'w': //wireframe
         cameraPos = cameraPos + cameraDir * 0.1;
         break;

      case 'a':
         cameraPos = cameraPos - right * 0.1;
         break;
      case 'd':
         cameraPos = cameraPos + right * 0.1;
         break;
      case 's':
         cameraPos = cameraPos - cameraDir * 0.1;
         break;
      case 'r':
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
      case '-':
         cameraDir = rotateY(cameraDir, -0.5);
		break;
      case '+':
         cameraDir = rotateY(cameraDir, 0.5);
         break;
      case '/':
         cameraDir = rotateX(cameraDir, -0.5);
         break;
      case '*':
         cameraDir = rotateX(cameraDir, 0.5);
         break;

         case '1':
         cameraPos.y += 0.05;
         break;

      case '2':
         cameraPos.y -= 0.05;
         break;
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
   generateHeightMap();
   generateCylinderOnTerrain();

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
