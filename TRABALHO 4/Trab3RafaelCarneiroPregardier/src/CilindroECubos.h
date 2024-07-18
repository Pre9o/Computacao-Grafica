#ifndef CILINDROECUBOS_H
#define CILINDROECUBOS_H

#include "Pecas.h" 

/**
 * @brief Classe responsável criar e desenhar um cilindro
 * 
 * A classe CilindroECubos é responsável por criar e desenhar um cilindro. Ela herda da classe Pecas e contém métodos para desenhar a base do cilindro, desenhar as arestas verticais do cilindro, desenhar o cilindro e gerar os vértices do cilindro.
 * 
 */
class CilindroECubos : public Pecas{
private:
    void DesenharBase(int inicio, int fim, int offset);
    void DesenharArestasVerticais();
    void DrawCilindro();

public:
    float raio;
    float altura;
    int numeroDeLados;

    CilindroECubos(Vector3 posicao, Vector3 rotacaoAplicada, float raio, float altura, int numeroDeLados);
    void GerarVertices();
    void aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia);
};

#endif 