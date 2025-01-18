#include <GL/glut.h>
#include "Vector3.h"
#include "heightMap.h"

#define TAMANHO_MAPA 100
#define NUM_TREES 6

/**
 * @brief  Desenha as faces do tetraedro quando ele chegar a profundidade desejada
 * 
 * @param a  Ponto A
 * @param b  Ponto B
 * @param c  Ponto C
 * @param normalA  Normal do ponto A
 * @param normalB  Normal do ponto B
 * @param normalC  Normal do ponto C
 */ 
void drawTriangle(float *a, float *b, float *c, Vector3 normalA, Vector3 normalB, Vector3 normalC);

/**
 * @brief  Desenha os cilindros na superfície do terreno
 * 
 * @param cilindros  Vetor de cilindros
 * @param cameraPos  Posição da câmera
 * @param specular  Especular
 * @param shininess  Brilho
 */
void drawCylindersOnTerrain(Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 cameraPos, GLfloat specular[], GLfloat shininess[]);

/**
 * @brief  Desenha o mapa de altura
 * 
 * @param heightMap  Mapa de altura
 * @param normalsHeightMap  Normais do mapa de altura
 * @param cilindros  Cilindros
 * @param cameraPos  Posição da câmera
 * @param specular  Especular
 * @param shininess  Brilho
 */
void drawHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[100][100], Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 cameraPos, GLfloat specular[], GLfloat shininess[]);
