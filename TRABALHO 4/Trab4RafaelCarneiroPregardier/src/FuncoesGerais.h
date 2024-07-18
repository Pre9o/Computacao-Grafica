#ifndef FUNCOESGERAIS3D_H
#define FUNCOESGERAIS3D_H

#include "CilindroECubos.h"
#include "Engrenagem.h"

/**
 * @brief Classe responsável por gerar as peças do motor
 * 
 * A classe FuncoesGerais3D é responsável por gerar as peças do motor, como o virabrequim, o pistão e o cilindro. Ela também é responsável por executar o motor 3D.
 * 
 */
class FuncoesGerais3D {
public:
    void setxParaPistao(float xParaPistao);
    void setyParaPistao(float yParaPistao);
    float getxParaPistao();
    float getyParaPistao();
    void gerarConexaoVirabrequim(Pecas *pecas[]);
    void gerarPistaoECilindro(Pecas *pecas[]);
    void executar3D(Pecas *pecas[], Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia, float velocidadeRotacao);
    void definirPecas(Pecas *pecas[]);

private:
    float xParaPistao;
    float yParaPistao;
};

#endif // FUNCOESGERAIS3D_H