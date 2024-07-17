#include "CilindroECubos.h"
#include <cmath> // Para funções matemáticas como cos() e sin()

// Implementação do método DesenharBase
void CilindroECubos::DesenharBase(int inicio, int fim, int offset) {
    for (int i = inicio; i < fim; i++) {
        CV::line(vertices2D[i], vertices2D[(i + 1) % numeroDeLados + offset]);
    }
}

// Implementação do método DesenharArestasVerticais
void CilindroECubos::DesenharArestasVerticais() {
    for (int i = 0; i < numeroDeLados; i++) {
        CV::line(vertices2D[i], vertices2D[i + numeroDeLados]);
    }
}

// Implementação do método DrawCilindro
void CilindroECubos::DrawCilindro() {
    DesenharBase(0, numeroDeLados, 0);
    DesenharBase(numeroDeLados, numeroDeLados * 2, numeroDeLados);
    DesenharArestasVerticais();

    vertices2D.clear();
    vertices3D.clear();
}

// Implementação do construtor
CilindroECubos::CilindroECubos(Vector3 posicao, Vector3 rotacaoAplicada, float raio, float altura, int numeroDeLados) : Pecas(posicao, rotacaoAplicada) {
    this->raio = raio;
    this->altura = altura;
    this->numeroDeLados = numeroDeLados;
}

// Implementação do método GerarVertices
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

// Implementação do método aplicarPipelineParaDesenho
void CilindroECubos::aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    GerarVertices();
    Pipeline(vertices3D, posicaoCamera, rotacaoCamera, distancia);
    DrawCilindro();
}