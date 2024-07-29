#include "trees.h"
#include "draw.h"

void generateCylinderOnTerrain(Vector3 cilindros[NUM_TREES][NUM_TREES], Vector3 controlPoints[4][4]){
   for(int i = 0; i < NUM_TREES - 1; i++)
   {
      for(int j = 0; j < NUM_TREES - 1; j++)
      {
         float x = i * 3.0 / NUM_TREES + rand() % TAMANHO_MAPA / 100.0;
         float z = j * 3.0 / NUM_TREES + rand() % TAMANHO_MAPA / 100.0;
         Vector3 teste = calculateHeight(x, z, controlPoints);

         cilindros[i][j] = Vector3(teste.x, teste.y, teste.z);
      }
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
    float v[4][3] = {
        { 1 , 0, float((-1 / sqrt(2)))  }, 
        { -1  , 0, float((-1 / sqrt(2))) }, 
        { 0, 1  , float((1 / sqrt(2))) }, 
        { 0, -1  , float((1 / sqrt(2))) } 
    };
    
    Vector3 normalsSphere[4];

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