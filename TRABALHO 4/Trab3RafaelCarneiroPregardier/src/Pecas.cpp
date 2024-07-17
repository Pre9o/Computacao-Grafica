#include "Pecas.h"

Pecas::Pecas(Vector3 posicao, Vector3 rotacaoAplicada) {
    this->posicao = posicao;
    this->rotacaoAplicada = rotacaoAplicada;
}

void Pecas::Pipeline(std::vector<Vector3> vertices3D, Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    std::vector<Vector2> vertices2D;

    for(size_t i = 0; i < vertices3D.size(); i++) {
        Vector3 ponto = vertices3D[i];
        ponto = Rotacao(ponto, rotacaoAplicada.x, rotacaoAplicada.y, rotacaoAplicada.z);
        
        ponto = ponto + posicao;

        ponto = WorldToCamera(ponto, posicaoCamera, rotacaoCamera);

        ponto = Translacao(ponto, distancia);

        Vector2 ponto2D = Projecao(ponto, distancia);

        vertices2D.push_back(ponto2D);
    }

    this->vertices2D = vertices2D;
}

void Pecas::aplicarRotacaoDoModelo(float velocidadeRotacao) {
    rotacaoAplicada.z -= velocidadeRotacao;
}

void Pecas::aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    Pipeline(vertices3D, posicaoCamera, rotacaoCamera, distancia);
}

Vector3 Pecas::Rotacao(Vector3 ponto, float angulo_x, float angulo_y, float angulo_z) {
    float x = ponto.x;
    float y = ponto.y;
    float z = ponto.z;

    // Rotação em torno do eixo X
    ponto.y = y * cos(angulo_x) - z * sin(angulo_x);
    ponto.z = y * sin(angulo_x) + z * cos(angulo_x);

    y = ponto.y;
    z = ponto.z;

    // Rotação em torno do eixo Y
    ponto.x = x * cos(angulo_y) + z * sin(angulo_y);
    ponto.z = -x * sin(angulo_y) + z * cos(angulo_y);

    x = ponto.x;
    z = ponto.z;

    // Rotação em torno do eixo Z
    ponto.x = x * cos(angulo_z) - y * sin(angulo_z);
    ponto.y = x * sin(angulo_z) + y * cos(angulo_z);

    return ponto;
}

Vector3 Pecas::WorldToCamera(Vector3 ponto, Vector3 posicaoCamera, Vector3 rotacaoCamera) {
    ponto.x -= posicaoCamera.x;
    ponto.y -= posicaoCamera.y;
    ponto.z -= posicaoCamera.z;

    ponto = Rotacao(ponto, rotacaoCamera.x, rotacaoCamera.y, rotacaoCamera.z);

    return ponto;
}

Vector2 Pecas::Projecao(Vector3 ponto, float distancia) {
    float x = ponto.x * distancia / ponto.z;
    float y = ponto.y * distancia / ponto.z;

    return Vector2{x, y};
}

Vector3 Pecas::Translacao(Vector3 ponto, float z) {
    ponto.z += z;

    return ponto;
}