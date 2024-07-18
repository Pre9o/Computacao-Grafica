#include "CilindroECubos.h"
#include <cmath> 

/**
 * Desenha a base do cilindro
 * 
 * @param inicio int 
 * @param fim int
 * @param offset int
 * 
 * @return void
 * 
 */
void CilindroECubos::DesenharBase(int inicio, int fim, int offset) {
    for (int i = inicio; i < fim; i++) {
        CV::color(1, 1, 1);
        CV::line(vertices2D[i], vertices2D[(i + 1) % numeroDeLados + offset]);
    }
}

/**
 * Desenha as arestas verticais do cilindro
 * 
 * @return void
 * 
 */
void CilindroECubos::DesenharArestasVerticais() {
    for (int i = 0; i < numeroDeLados; i++) {
        CV::color(1, 1, 1);
        CV::line(vertices2D[i], vertices2D[i + numeroDeLados]);
    }
}

/**
 * Desenha o cilindro
 * 
 * @return void
 * 
 */
void CilindroECubos::DrawCilindro() {
    DesenharBase(0, numeroDeLados, 0);
    DesenharBase(numeroDeLados, numeroDeLados * 2, numeroDeLados);
    DesenharArestasVerticais();

    vertices2D.clear();
    vertices3D.clear();
}

/**
 * Construtor da classe CilindroECubos
 * 
 * @param posicao Vector3
 * @param rotacaoAplicada Vector3
 * @param raio float
 * @param altura float
 * @param numeroDeLados int
 * 
 */
CilindroECubos::CilindroECubos(Vector3 posicao, Vector3 rotacaoAplicada, float raio, float altura, int numeroDeLados) : Pecas(posicao, rotacaoAplicada) {
    this->raio = raio;
    this->altura = altura;
    this->numeroDeLados = numeroDeLados;
}

/**
 * Gera os vertices do cilindro
 * 
 * @return void
 * 
 */
void CilindroECubos::GerarVertices() {
    float anguloRad = (2 * M_PI / numeroDeLados);

    for (int i = 0; i < numeroDeLados; i++) {
        float x = raio * cos(anguloRad * i);
        float y = 0;
        float z = raio * sin(anguloRad * i);

        vertices3D.push_back(Vector3(x, y, z));
    }

    for (int i = 0; i < numeroDeLados; i++) {
        float x = raio * cos(anguloRad * i);
        float y = altura;
        float z = raio * sin(anguloRad * i);

        vertices3D.push_back(Vector3(x, y, z));
    }
}

/**
 * Aplica o pipeline para desenhar o cilindro
 * 
 * @param posicaoCamera Vector3
 * @param rotacaoCamera Vector3
 * @param distancia float
 * 
 * @return void
 * 
 */
void CilindroECubos::aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    GerarVertices();
    Pipeline(vertices3D, posicaoCamera, rotacaoCamera, distancia);
    DrawCilindro();
}