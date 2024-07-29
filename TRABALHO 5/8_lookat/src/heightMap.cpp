#include "heightMap.h"

Vector3 bezierPoint(Vector3* points, int numPoints, float t){
    if (numPoints == 1) {
         return points[0];
      }
      Vector3* tempPoints = new Vector3[numPoints - 1];
      for (int i = 0; i < numPoints - 1; i++) {
         tempPoints[i] = points[i] * (1 - t) + points[i + 1] * t;
      }
      Vector3 result = bezierPoint(tempPoints, numPoints - 1, t);
      delete[] tempPoints;
      return result;
}

Vector3 bezierSurfacePoint(Vector3 points[PONTOS_BEZIER][PONTOS_BEZIER], float u, float v){
    Vector3 tempPoints[PONTOS_BEZIER];
    for (int i = 0; i < PONTOS_BEZIER; i++) {
        tempPoints[i] = bezierPoint(points[i], PONTOS_BEZIER, u);
    }
    return bezierPoint(tempPoints, PONTOS_BEZIER, v);
}

Vector3 calculateHeight(float x, float z, Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER]){
    float u = x/3;
    float v = z/3;
      return bezierSurfacePoint(controlPoints, u, v) * 3;
   }

void calculateNormalsHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA]){
    for (int i = 0; i < TAMANHO_MAPA - 1; i++) {
        for (int j = 0; j < TAMANHO_MAPA - 1; j++) {
            Vector3 normal1 = calculateNormal(heightMap[i][j], heightMap[i+1][j], heightMap[i][j+1]);
            Vector3 normal2 = calculateNormal(heightMap[i+1][j], heightMap[i+1][j+1], heightMap[i][j+1]);

            normalsHeightMap[i][j] = normal1;
            normalsHeightMap[i][j+1] = normal1;
            normalsHeightMap[i+1][j] = normal2;
            normalsHeightMap[i+1][j+1] = normal2;
        }
    }
}

void generateHeightMap(Vector3 heightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 normalsHeightMap[TAMANHO_MAPA][TAMANHO_MAPA], Vector3 controlPoints[PONTOS_BEZIER][PONTOS_BEZIER]){
    for (int i = 0; i < TAMANHO_MAPA; i++) {
        for (int j = 0; j < TAMANHO_MAPA; j++) {
            float u = i / 99.0;
            float v = j / 99.0; 
            heightMap[i][j] = bezierSurfacePoint(controlPoints, u, v) * 3;
        }
    }

    calculateNormalsHeightMap(heightMap, normalsHeightMap);
}
