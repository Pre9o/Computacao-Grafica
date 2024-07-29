#include "Vector3.h"

#define PI 3.14159265

/**
 * @brief Calcula o vetor normal
 * 
 * @param v1  Vetor 1
 * @param v2  Vetor 2
 * @param v3  Vetor 3
 * @return Vector3 
 */
Vector3 calculateNormal(Vector3 v1, Vector3 v2, Vector3 v3);

/**
 * @brief  Rotaciona o vetor v em torno do eixo X
 * 
 * @param v  Vetor a ser rotacionado
 * @param angle  Angulo de rotacao
 * @return Vector3 
 */
Vector3 rotateX(Vector3 v, float angle);

/**
 * @brief  Rotaciona o vetor v em torno do eixo Y
 * 
 * @param v  Vetor a ser rotacionado
 * @param angle  Angulo de rotacao
 * @return Vector3 
 */
Vector3 rotateY(Vector3 v, float angle);

/**
 * @brief  Rotaciona o vetor v em torno do eixo Z
 * 
 * @param v  Vetor a ser rotacionado
 * @param angle  Angulo de rotacao
 * @return Vector3 
 */
Vector3 rotateZ(Vector3 v, float angle);

/**
 * @brief  Normaliza o ponto
 * 
 * @param x  Coordenada x
 * @param y  Coordenada y
 * @param z  Coordenada z
 */
void normalizePoint(float &x, float &y, float &z);

/**
 * @brief  Calcula a distancia entre dois pontos
 * 
 * @param point  Ponto 1
 * @param cameraPos  Ponto 2
 * @return float 
 */
float calculateDistance(Vector3 point, Vector3 cameraPos);
