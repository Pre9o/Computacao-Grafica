#include "FuncoesGerais.h"
#include <cmath> // Para funções matemáticas como sin, cos, atan2

// Definições dos métodos de FuncoesGerais3D

void FuncoesGerais3D::setxParaPistao(float xParaPistao) {
    this->xParaPistao = xParaPistao;
}

void FuncoesGerais3D::setyParaPistao(float yParaPistao) {
    this->yParaPistao = yParaPistao;
}

float FuncoesGerais3D::getxParaPistao() {
    return this->xParaPistao;
}

float FuncoesGerais3D::getyParaPistao() {
    return this->yParaPistao;
}

void FuncoesGerais3D::gerarConexaoVirabrequim(Pecas *pecas[]) {
    float angulo = pecas[2]->rotacaoAplicada.z + M_PI;
    float xParaPistao = -200 * cos(angulo);
    float yParaPistao = -200 * sin(angulo);
    pecas[4] = new CilindroECubos(Vector3(xParaPistao, yParaPistao, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);

    setxParaPistao(xParaPistao);
    setyParaPistao(yParaPistao);
}

void FuncoesGerais3D::gerarPistaoECilindro(Pecas *pecas[]) {
    float offset = 3 * M_PI / 2;
    float pistonToCylinderAngle = atan2(600 - this->yParaPistao, 0 - this->xParaPistao) + offset;
    
    pecas[4] = new CilindroECubos(Vector3(this->xParaPistao, this->yParaPistao, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);
    pecas[7] = new CilindroECubos(Vector3(0, 600, -250), Vector3(0, 0, pistonToCylinderAngle + M_PI), 25, 400, 20);
    pecas[8] = new CilindroECubos(Vector3(this->xParaPistao, this->yParaPistao, -250), Vector3(0, 0, pistonToCylinderAngle), 15, 400, 20);
}

void FuncoesGerais3D::executar3D(Pecas *pecas[], Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia, float velocidadeRotacao) {
    for(int i = 0; i < 7; i++) {
        if(i == 6) {
            pecas[i]->aplicarRotacaoDoModelo(-velocidadeRotacao);
        } else {
            pecas[i]->aplicarRotacaoDoModelo(velocidadeRotacao);
        }
    }

    this->gerarConexaoVirabrequim(pecas);
    this->gerarPistaoECilindro(pecas);

    for(int i = 0; i < 9; i++) {
        pecas[i]->aplicarPipelineParaDesenho(posicaoCamera, rotacaoCamera, distancia);
    }
}