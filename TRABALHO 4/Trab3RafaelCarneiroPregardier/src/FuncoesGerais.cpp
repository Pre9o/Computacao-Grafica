#include "FuncoesGerais.h"
#include <cmath> 

/**
 * Funcao que seta o valor de xParaPistao
 * 
 * @param xParaPistao float
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::setxParaPistao(float xParaPistao){
    this->xParaPistao = xParaPistao;
}

/**
 * Funcao que seta o valor de yParaPistao
 * 
 * @param yParaPistao float
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::setyParaPistao(float yParaPistao){
    this->yParaPistao = yParaPistao;
}

/**
 * Funcao que retorna o valor de xParaPistao
 * 
 * @return float
 * 
 */
float FuncoesGerais3D::getxParaPistao(){
    return this->xParaPistao;
}

/**
 * Funcao que retorna o valor de yParaPistao
 * 
 * @return float
 * 
 */
float FuncoesGerais3D::getyParaPistao(){
    return this->yParaPistao;
}

/**
 * Funcao que gera a conexao entre o virabrequim e o pistao
 * 
 * @param pecas Pecas
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::gerarConexaoVirabrequim(Pecas *pecas[]){
    float angulo = pecas[2]->rotacaoAplicada.z + M_PI;
    float xParaPistao = -200 * cos(angulo);
    float yParaPistao = -200 * sin(angulo);
    pecas[4] = new CilindroECubos(Vector3(xParaPistao, yParaPistao, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);

    setxParaPistao(xParaPistao);
    setyParaPistao(yParaPistao);
}

/**
 * Funcao que gera o pistao e o cilindro
 * 
 * @param pecas Pecas
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::gerarPistaoECilindro(Pecas *pecas[]){
    float offset = 3 * M_PI / 2;
    float pistonToCylinderAngle = atan2(600 - this->yParaPistao, 0 - this->xParaPistao) + offset;

    pecas[4] = new CilindroECubos(Vector3(this->xParaPistao, this->yParaPistao, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);
    pecas[7] = new CilindroECubos(Vector3(0, 600, -250), Vector3(0, 0, pistonToCylinderAngle + M_PI), 25, 400, 20);
    pecas[8] = new CilindroECubos(Vector3(this->xParaPistao, this->yParaPistao, -250), Vector3(0, 0, pistonToCylinderAngle), 15, 400, 20);
}

/**
 * Funcao que executa o 3D
 * 
 * @param pecas Pecas
 * @param posicaoCamera Vector3
 * @param rotacaoCamera Vector3
 * @param distancia float
 * @param velocidadeRotacao float
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::executar3D(Pecas *pecas[], Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia, float velocidadeRotacao) {
    for(int i = 0; i < 7; i++){
        if(i == 6) {
            pecas[i]->aplicarRotacaoDoModelo(-velocidadeRotacao);
        } else {
            pecas[i]->aplicarRotacaoDoModelo(velocidadeRotacao);
        }
    }

    this->gerarConexaoVirabrequim(pecas);
    this->gerarPistaoECilindro(pecas);

    for(int i = 0; i < 9; i++){
        pecas[i]->aplicarPipelineParaDesenho(posicaoCamera, rotacaoCamera, distancia);
    }
}

/**
 * Funcao que define as pecas
 * 
 * @param pecas Pecas
 * 
 * @return void
 * 
 */
void FuncoesGerais3D::definirPecas(Pecas *pecas[]){
    pecas[0] = new CilindroECubos(Vector3(0, 0, 0), Vector3(M_PI / 2, 0, 0), 20, 300, 20);
    pecas[1] = new CilindroECubos(Vector3(0, 0, -180), Vector3(M_PI / 2, M_PI / 2, 0), 10, 200, 4);
    pecas[2] = new CilindroECubos(Vector3(0, 0, -200 + 10), Vector3(M_PI / 2, 0, 0), 10, 200, 4);
    pecas[3] = new CilindroECubos(Vector3(0, 0, -100), Vector3(M_PI / 2, M_PI / 2, 0), 10, 200, 4);
    pecas[4] = new CilindroECubos(Vector3(0, 0, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);
    pecas[5] = new Engrenagem(Vector3(0, 0, 200), Vector3(0, 0, 0), 80, 100, 120);
    pecas[6] = new Engrenagem(Vector3(2 * 120 - 20, 0, 200), Vector3(0, 0, 0), 80, 100, 120);
}