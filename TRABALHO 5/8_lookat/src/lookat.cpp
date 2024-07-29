#include <GL/glut.h>

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "heightMap.h"
#include "trees.h"
#include "draw.h"

#define SCREEN_X 1280
#define SCREEN_Y 720

#define TAMANHO_MAPA 100
#define NUM_TREES 6
#define PONTOS_BEZIER 4

int polygonMode = 1;

float angLight = 0;
float abertura = 59.0;
float znear = 0.1;
float zfar = 5000;
float aspect = 16.0/9.0;
float pitch = 0.0f; 

GLfloat light_0_position[] = { 1, 1, 1, 0};
GLfloat light_0_difuse[] = { 1, 1, 1 };  
GLfloat light_0_ambient[] = { 0.4, 0.4, 0.4 }; 

GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat shininess[] = { 50.0 };

Vector3 cameraPos(1, 1, 1);
Vector3 cameraDir(0, 0, 0.1);
Vector3 up(0, 1, 0);

Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA];

Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA];
Vector3 normalsSphere[PONTOS_BEZIER];

Vector3 cilindros[NUM_TREES][NUM_TREES];

Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER] = {
        {{0, 0, 0}, {1, 0, 0}, {2, -2, 0}, {3, 0, 0}},
        {{0, 0, 0}, {1, 1, 0}, {2, -0, 0}, {3, 0, 0}},
        {{0, 0, 2}, {1, 0, 2}, {2, 0, 2}, {3, 0, 2}},
        {{0, 0, 3}, {1, 1, 3}, {2, -2, 3}, {3, 0, 3}}
    };

struct KeyboardState {
    bool W = false;
    bool A = false;
    bool S = false;
    bool D = false;
    bool Space = false;
    bool Shift = false;
    bool LeftArrow = false;
    bool RightArrow = false;
    bool UpArrow = false;
    bool DownArrow = false;
} keyboardState;


void updateCameraPosition(){
   if(keyboardState.W){
      cameraPos = cameraPos + cameraDir * 0.1;
   }
   if(keyboardState.A){
      Vector3 right = cameraDir.cross(up);
      right.normalize();
      cameraPos = cameraPos - right * 0.1;
   }
   if(keyboardState.S){
      cameraPos = cameraPos - cameraDir * 0.1;
   }
   if(keyboardState.D){
      Vector3 right = cameraDir.cross(up);
      right.normalize();
      cameraPos = cameraPos + right * 0.1;
   }
   if(keyboardState.Space){
      cameraPos.y += 0.01;
   }
   if(keyboardState.Shift){
      cameraPos.y -= 0.01;
   }
   if(keyboardState.LeftArrow){
      cameraDir = rotateY(cameraDir, 0.5);
   }
   if(keyboardState.RightArrow){
      cameraDir = rotateY(cameraDir, -0.5);
   }
   if(keyboardState.UpArrow){
      if(pitch > -90.0f){ 
         Vector3 right = cameraDir.cross(up);
         right.normalize();
         cameraDir = rotateX(cameraDir, -0.5); 
         pitch -= 0.5; 
      }
   }
   if (keyboardState.DownArrow) {
      if(pitch < 90.0f){
         Vector3 right = cameraDir.cross(up);
         right.normalize();
         cameraDir = rotateX(cameraDir, 0.5); 
         pitch += 0.5; 
      }
   }
}

void init(){
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_0_difuse);
   glLightfv(GL_LIGHT0, GL_AMBIENT,  light_0_ambient);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_FRONT);

   glClearColor(0.5294117, 0.807843, 0.9803921, 1);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glEnable(GL_DEPTH_TEST);
}

void display(void){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );

   gluPerspective(abertura, aspect, znear, zfar);

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity( );

   cameraDir.normalize();
   gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + cameraDir.x, cameraPos.y + cameraDir.y, cameraPos.z + cameraDir.z, up.x, up.y, up.z);

   light_0_position[0] = NUM_TREES * sin(angLight);
   light_0_position[1] = PONTOS_BEZIER;
   light_0_position[2] = PONTOS_BEZIER + NUM_TREES * cos(angLight); 
   angLight += 0.004;

   glPushMatrix();
      glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_0_difuse);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  light_0_ambient);
      glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
      glLightfv(GL_LIGHT0, GL_SHININESS, shininess);

      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_0_difuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_0_ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
      glTranslated(light_0_position[0], light_0_position[1],light_0_position[2]);
      glutSolidSphere (1.0, 20, 16);

   glPopMatrix();

   drawHeightMap(heightMap, normalsHeightMap, cilindros, cameraPos, specular, shininess);
   updateCameraPosition();

   glutSwapBuffers();
}

void handleKeyboard(unsigned char key, bool isPressed) {
    switch(tolower(key)){
        case 'w': keyboardState.W = isPressed; break;
        case 'a': keyboardState.A = isPressed; break;
        case 's': keyboardState.S = isPressed; break;
        case 'd': keyboardState.D = isPressed; break;
        case ' ': keyboardState.Space = isPressed; break;
        case 212: keyboardState.Shift = isPressed; break;
        case 200: keyboardState.LeftArrow = isPressed; break;
        case 201: keyboardState.UpArrow = isPressed; break;
        case 202: keyboardState.RightArrow = isPressed; break;
        case 203: keyboardState.DownArrow = isPressed; break;
        case 27: if (isPressed) exit(0); break;
        case 'p':
            if(isPressed){
                polygonMode = !polygonMode;
                glPolygonMode(GL_FRONT_AND_BACK, polygonMode ? GL_LINE : GL_FILL);
            }
            break;
    }
}

void keyboard(unsigned char key, int x, int y){
   handleKeyboard(key, true);
}

void keyboardUp(unsigned char key, int , int){
   handleKeyboard(key, false);
}

void special(int key, int , int){
   handleKeyboard(key + 100, true);
}

void specialUp(int key, int , int){
  handleKeyboard(key + 100, false);
}

void MotionFunc(int x, int y){
}

void MouseFunc(int botao, int estado, int x, int y){
}

int main (){
   int argc = 0;
   glutInit(&argc, NULL);

   srand(time(NULL));
   
   generateHeightMap(heightMap, normalsHeightMap, controlPoints);
   generateCylinderOnTerrain(cilindros, controlPoints);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowSize (SCREEN_X, SCREEN_Y);
   glutInitWindowPosition (450, 10);

   glutCreateWindow ("Rafael Carneiro Pregardier");
   init();

   glutDisplayFunc(display);
   glutMotionFunc(MotionFunc);
   glutMouseFunc(MouseFunc);
   glutIdleFunc(display);
   glutKeyboardFunc(keyboard);
   glutKeyboardUpFunc(keyboardUp);
   glutSpecialFunc(special);
   glutSpecialUpFunc(specialUp);

   glutMainLoop();

   return 0;
}
