#include "anotherFunctions.h"

#define PONTOS_BEZIER 4
#define TAMANHO_MAPA 100
    
/**
 * @brief  Calcula um ponto de uma curva de bezier
 * 
 * @param points  Pontos da curva
 * @param numPoints  Numero de pontos
 * @param t  Parametro t
 * @return Vector3 
 */
Vector3 bezierPoint(Vector3* points, int numPoints, float t);

/**
 * @brief  Calcula um ponto de uma superficie de bezier
 * 
 * @param points  Pontos da superficie
 * @param u  Parametro u
 * @param v  Parametro v
 * @return Vector3 
 */
Vector3 bezierSurfacePoint(Vector3 points[PONTOS_BEZIER][PONTOS_BEZIER], float u, float v);

/**
 * @brief  Calcula a altura de um ponto
 * 
 * @param x  Coordenada x
 * @param z  Coordenada z
 * @param controlPoints  Pontos de controle
 * @return Vector3 
 */
Vector3 calculateHeight(float x, float z, Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER]);

/**
 * @brief  Calcula os vetores normais de um mapa de altura
 * 
 * @param heightMap  Mapa de altura
 * @param normalsHeightMap  Mapa de vetores normais
 */
void calculateNormalsHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA]);

/**
 * @brief  Gera um mapa de altura
 * 
 * @param heightMap  Mapa de altura
 * @param normalsHeightMap  Mapa de vetores normais
 * @param controlPoints  Pontos de controle
 */
void generateHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER]);