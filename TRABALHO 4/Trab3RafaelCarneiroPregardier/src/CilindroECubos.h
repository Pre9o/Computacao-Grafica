#ifndef CILINDROECUBOS_H
#define CILINDROECUBOS_H

#include "Pecas.h" // Ajuste o nome do arquivo de cabeçalho conforme necessário

class CilindroECubos : public Pecas {
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

#endif // CILINDROECUBOS_H