#ifndef FUNCOESGERAIS3D_H
#define FUNCOESGERAIS3D_H

#include "CilindroECubos.h"
#include "Engrenagem.h"

class FuncoesGerais3D {
public:
    void setxParaPistao(float xParaPistao);
    void setyParaPistao(float yParaPistao);
    float getxParaPistao();
    float getyParaPistao();
    void gerarConexaoVirabrequim(Pecas *pecas[]);
    void gerarPistaoECilindro(Pecas *pecas[]);
    void executar3D(Pecas *pecas[], Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia, float velocidadeRotacao);

private:
    float xParaPistao;
    float yParaPistao;
};

#endif // FUNCOESGERAIS3D_H