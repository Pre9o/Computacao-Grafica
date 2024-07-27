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

float angLight = 0;

float abertura = 59.0;
float znear  = 0.1;
float zfar   = 5000;
float aspect = 16.0/9.0;

GLfloat mat_diffuse_1[] = {1, 1 ,1}; //definicao do material para esfera 1
GLfloat mat_diffuse_2[] = {0, 1 ,0}; //definicao do material para esfera 2

GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat shininess[] = { 50.0 };

GLfloat terrain_ambient[] = { 0.2, 1, 0.2, 1.0 };
GLfloat terrain_diffuse[] = { 0.6, 1, 0.2, 1.0 };

GLfloat log_ambient[] = { 0.6, 0.3, 0.1, 1.0 };
GLfloat log_diffuse[] = { 0.6, 0.3, 0.1, 1.0 };

GLfloat leaf_ambient[] = { 0.0, 0.5, 0.0, 1.0 };
GLfloat leaf_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };

GLfloat light_0_position[] = { 1, 1, 1, 0};
GLfloat light_0_difuse[]   = { 1, 1, 1 };  //luz branca
GLfloat light_0_ambient[]  = { 0.4, 0.4, 0.4 }; //branco

Vector3 cameraPos(1, 1, 1);
Vector3 cameraDir(0, 0, 0.1);
Vector3 up(0, 1, 0);

Vector3 heightMap[100][100];

Vector3 normalsHeightMap[100][100];
Vector3 normalsSphere[4];


Vector3 cilindros[6][6];

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

Vector3 calculateNormal(Vector3 v1, Vector3 v2, Vector3 v3) {
    Vector3 normal;
    Vector3 u = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    Vector3 v = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

    normal.x = u.y * v.z - u.z * v.y;
    normal.y = u.z * v.x - u.x * v.z;
    normal.z = u.x * v.y - u.y * v.x;

    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length != 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }

    // Invertendo as normais
    normal.x = -normal.x;
    normal.y = -normal.y;
    normal.z = -normal.z;

    return normal;
}

void calculateNormalsHeightMap() {
    for (int i = 0; i < 100 - 1; i++) {
        for (int j = 0; j < 100 - 1; j++) {
            // Ajuste a ordem dos vértices para garantir que as normais estejam corretas
            Vector3 normal1 = calculateNormal(heightMap[i][j], heightMap[i+1][j], heightMap[i][j+1]);
            Vector3 normal2 = calculateNormal(heightMap[i+1][j], heightMap[i+1][j+1], heightMap[i][j+1]);

            normalsHeightMap[i][j] = normal1;
            normalsHeightMap[i][j+1] = normal1;
            normalsHeightMap[i+1][j] = normal2;
            normalsHeightMap[i+1][j+1] = normal2;
        }
    }
}


// Modificar a função generateHeightMap para usar a superfície de Bézier
void generateHeightMap() {
    // Usar os pontos de controle para gerar o mapa de altura
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            float u = i / 99.0; // Normaliza i para o intervalo [0, 1]
            float v = j / 99.0; // Normaliza j para o intervalo [0, 1]
            heightMap[i][j] = bezierSurfacePoint(controlPoints, u, v) * 3;
        }
    }

    calculateNormalsHeightMap();
}

// Função para calcular a altura de um ponto no terreno
Vector3 calculateHeight(float x, float z) {
      // Calcula a posição do ponto no intervalo [0, 1]

      float u = x/3;
      float v = z/3;
      // Calcula a altura do ponto na superfície de Bézier
      return bezierSurfacePoint(controlPoints, u, v) * 3;
   }


void generateCylinderOnTerrain(){
   for(int i=1; i<6; i++)
   {
      for(int j=1; j<6; j++)
      {
         float x = i * 3.0 / 6;
         float z = j * 3.0 / 6;
         Vector3 teste = calculateHeight(x, z);

         cilindros[i][j] = Vector3(teste.x, teste.y, teste.z);
      }

  }
}

void drawTriangle(float *a, float *b, float *c, Vector3 normalA, Vector3 normalB, Vector3 normalC) {
   if(a == b || a == c || b == c) {
      return;
   }
    
   glBegin(GL_TRIANGLES);
      
      glNormal3f(normalC.x, normalC.y, normalC.z);

      glVertex3fv(c);

      glNormal3f(normalB.x, normalB.y, normalB.z);

      glVertex3fv(b);

      glNormal3f(normalA.x, normalA.y, normalA.z);

      glVertex3fv(a);

      
   glEnd();
}

void normalizePoint(float &x, float &y, float &z) {
    float length = sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

void divideFace(float *a, float *b, float *c, Vector3 normalA, Vector3 normalB, Vector3 normalC, int depth, float edgeSize) {
    if (depth == 0) {
        drawTriangle(a, b, c, normalA, normalB, normalC);
    } else {
        float ab[3], ac[3], bc[3];
        Vector3 normalAB, normalAC, normalBC;

        for (int i = 0; i < 3; i++) {
            ab[i] = (a[i] + b[i]) / 2;
            ac[i] = (a[i] + c[i]) / 2;
            bc[i] = (b[i] + c[i]) / 2;
        }
        
        normalizePoint(ab[0], ab[1], ab[2]);
        normalizePoint(ac[0], ac[1], ac[2]);
        normalizePoint(bc[0], bc[1], bc[2]);

        normalAB = Vector3(ab[0], ab[1], ab[2]);

         normalAC = Vector3(ac[0], ac[1], ac[2]);

         normalBC = Vector3(bc[0], bc[1], bc[2]);

        for(int i = 0; i < 3; i++) {
            ab[i] *= edgeSize;
            ac[i] *= edgeSize;
            bc[i] *= edgeSize;
        }


        divideFace(a, ab, ac, normalA, normalAB, normalAC, depth - 1, edgeSize);
        divideFace(ab, b, bc, normalAB, normalB, normalBC, depth - 1, edgeSize);
        divideFace(ab, bc, ac, normalAB, normalBC, normalAC, depth - 1, edgeSize);
        divideFace(ac, bc, c, normalAC, normalBC, normalC, depth - 1, edgeSize);
    }
}

void drawSphereFromTetrahedron(int depth, float edgeSize) {
    // Pontos base do tetraedro
    float v[4][3] = {
        { 1 , 0, float((-1 / sqrt(2)))  }, // Topo
        { -1  , 0, float((-1 / sqrt(2))) }, // Base esquerda
        { 0, 1  , float((1 / sqrt(2))) }, // Base direita
        { 0, -1  , float((1 / sqrt(2))) } // Base frente
    };

    for(int i = 0; i < 4; i++) {
        normalizePoint(v[i][0], v[i][1], v[i][2]);
        Vector3 vertex = Vector3(v[i][0], v[i][1], v[i][2]);
        normalsSphere[i] = vertex;
        for (int j = 0; j < 3; j++) {
            v[i][j] *= edgeSize;
        }
      }

    divideFace(v[0], v[1], v[2], normalsSphere[0], normalsSphere[1], normalsSphere[2], depth, edgeSize);
    divideFace(v[0], v[1], v[3], normalsSphere[0], normalsSphere[1], normalsSphere[3], depth, edgeSize);
    divideFace(v[0], v[2], v[3], normalsSphere[0], normalsSphere[2], normalsSphere[3], depth, edgeSize);
    divideFace(v[1], v[2], v[3], normalsSphere[1], normalsSphere[2], normalsSphere[3], depth, edgeSize);
}


float calculateDistance(Vector3 point){
   return sqrt(cameraPos.x * point.x + cameraPos.y * point.y + cameraPos.z * point.z);
}


void drawCylindersOnTerrain(){
   for(int i=1; i<6; i++)
   {
      for(int j=1; j<6; j++)
      {
         GLUquadric *quadric = gluNewQuadric();

         glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, log_diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, log_ambient);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glColor3f(0.545098, 0.270588, 0.07450980); // Cor do tronco
            glTranslatef(cilindros[i][j].x, cilindros[i][j].y, cilindros[i][j].z);
            glRotatef(270, 1, 0, 0); // Rotaciona para que o cilindro fique de pé
            gluCylinder(quadric, 0.02, 0.02, 0.2, 10, 10); // Desenha o cilindro
         glPopMatrix();

         // Desenha o tetraedro no topo do cilindro
         glPushMatrix();
            glColor3f(0.0, 1.0, 0.0); // Cor da copa da árvore (verde)
            glMaterialfv(GL_FRONT, GL_AMBIENT, leaf_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            // Ajusta a posição para o topo do cilindro. Ajuste o 0.2 conforme a altura do seu cilindro
            glTranslatef(cilindros[i][j].x, cilindros[i][j].y + 0.2, cilindros[i][j].z);
            if(i % 2 == 0 && j % 2 == 0) {
               if(calculateDistance(cilindros[i][j]) > 0.5){
                drawSphereFromTetrahedron(5, 0.1); // Desenha a copa da árvore
               }
               else{
                  drawSphereFromTetrahedron(1, 0.1); // Desenha a copa da árvore
               }
            }
            else if (i % 2 == 0) {
               glutSolidCube(0.1);
            }
            else{
               glutSolidTeapot(0.1);
            }
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
            glMaterialfv(GL_FRONT, GL_AMBIENT, terrain_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, terrain_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glNormal3f(normalsHeightMap[i][j+1].x, normalsHeightMap[i][j+1].y, normalsHeightMap[i][j+1].z);
            glVertex3f(heightMap[i][j+1].x, heightMap[i][j+1].y, heightMap[i][j+1].z);

            glNormal3f(normalsHeightMap[i][j].x, normalsHeightMap[i][j].y, normalsHeightMap[i][j].z);
            glVertex3f(heightMap[i][j].x, heightMap[i][j].y, heightMap[i][j].z);

            glNormal3f(normalsHeightMap[i+1][j+1].x, normalsHeightMap[i+1][j+1].y, normalsHeightMap[i+1][j+1].z);
            glVertex3f(heightMap[i+1][j+1].x, heightMap[i+1][j+1].y, heightMap[i+1][j+1].z);

            glNormal3f(normalsHeightMap[i+1][j].x, normalsHeightMap[i+1][j].y, normalsHeightMap[i+1][j].z);
            glVertex3f(heightMap[i+1][j].x, heightMap[i+1][j].y, heightMap[i+1][j].z);

            glEnd();
        }
   }
      drawCylindersOnTerrain();
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

   //seta os parametros fixos da luz. A posicao eh atualizada a cada frame.
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_0_difuse);
   glLightfv(GL_LIGHT0, GL_AMBIENT,  light_0_ambient);
   

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_CULL_FACE);

   //glCullFace(GL_FRONT);

   glClearColor(0.5294117, 0.807843, 0.9803921, 1);

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

   //desenha o mapa de altura

   light_0_position[0] = cos(angLight)*9; //x
   light_0_position[1] = sin(angLight)*9; //y
   light_0_position[2] = 0; //z

   glPushMatrix();
      glTranslated(100,0,1000);
      glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
   glPopMatrix();

   //desenha a fonte
   /*
   glTranslated(light_0_position[0], light_0_position[1],light_0_position[2]);
   glutSolidSphere (1.0, 20, 16);*/

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