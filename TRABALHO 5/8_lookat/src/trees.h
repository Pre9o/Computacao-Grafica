#include "Vector3.h"
#include <stdlib.h>
#include <math.h>

#define NUM_TREES 6
#define PONTOS_BEZIER 4

/**
 * @brief  Gera os cilindros na superfície do terreno
 * 
 * @param cilindros  Vetor de cilindros
 * @param controlPoints  Pontos de controle
 */
void generateCylinderOnTerrain(Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER]);

/**
 * @brief  Divide uma face em 4 faces menores
 * 
 * @param a  Ponto A
 * @param b  Ponto B
 * @param c  Ponto C
 * @param normalA  Normal do ponto A
 * @param normalB  Normal do ponto B
 * @param normalC  Normal do ponto C
 * @param depth  Profundidade
 * @param edgeSize  Tamanho da aresta
 */
void divideFace(float *a, float *b, float *c, Vector3 normalA, Vector3 normalB, Vector3 normalC, int depth, float edgeSize);

/**
 * @brief  Desenha uma esfera a partir de um tetraedro
 * 
 * @param depth  Profundidade
 * @param edgeSize  Tamanho da aresta
 */
void drawSphereFromTetrahedron(int depth, float edgeSize);
