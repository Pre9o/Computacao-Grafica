#include "draw.h"
#include "anotherFunctions.h"
#include "trees.h"
#include "heightMap.h"


GLfloat terrain_ambient[] = { 0.2, 1, 0.2, 1.0 };
GLfloat terrain_diffuse[] = { 0.6, 1, 0.2, 1.0 };

GLfloat log_ambient[] = { 0.6, 0.3, 0.1, 1.0 };
GLfloat log_diffuse[] = { 0.6, 0.3, 0.1, 1.0 };

GLfloat leaf_ambient[] = { 0.0, 0.5, 0.0, 1.0 };
GLfloat leaf_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };

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

void drawCylindersOnTerrain(Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 cameraPos, GLfloat specular[], GLfloat shininess[]) {
   for(int i = 0; i < NUM_TREES; i++)
   {
      for(int j = 0; j < NUM_TREES; j++)
      {
         GLUquadric *quadric = gluNewQuadric();

         glPushMatrix();
            glMaterialfv(GL_FRONT, GL_DIFFUSE, log_diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, log_ambient);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glColor3f(0.545098, 0.270588, 0.07450980); 
            glTranslatef(cilindros[i][j].x, cilindros[i][j].y, cilindros[i][j].z);
            glRotatef(270, 1, 0, 0);
            gluCylinder(quadric, 0.02, 0.02, 0.2, 10, 10); 
         glPopMatrix();

         glPushMatrix();
            glColor3f(0.0, 1.0, 0.0); 
            glMaterialfv(GL_FRONT, GL_AMBIENT, leaf_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glTranslatef(cilindros[i][j].x, cilindros[i][j].y + 0.2, cilindros[i][j].z);
            if(i % 2 == 0 && j % 2 == 0){
               if(calculateDistance(cilindros[i][j], cameraPos) < 5){
                drawSphereFromTetrahedron(5, 0.1);
               }
               else{
                  drawSphereFromTetrahedron(1, 0.1); 
               }
            }
            else if (i % 2 == 0){
               glutSolidCube(0.1);
            }
            else if(j % 2 == 0){
               glRotatef(270, 1, 0, 0);
               glutSolidCone(0.1, 0.2, 10, 10);
            }
            else{
               glutSolidTeapot(0.1);
            }
         glPopMatrix();
      }
   }
}

void drawHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 cameraPos, GLfloat specular[], GLfloat shininess[]) {
   for(int i = 0; i < TAMANHO_MAPA - 1; i++){
      for(int j = 0; j < TAMANHO_MAPA - 1; j++){
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
    drawCylindersOnTerrain(cilindros, cameraPos, specular, shininess);
}