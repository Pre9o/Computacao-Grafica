#ifndef ENGRENAGEM_H
#define ENGRENAGEM_H

#include "Pecas.h" // Supondo que Pecas seja uma classe definida em outro lugar
#include "Vector3.h" // Supondo que Vector3 seja uma classe/estrutura definida em outro lugar

class Engrenagem : public Pecas {
public:
    int numeroDeDentes;
    float raioMenor;
    float raioMaior;

    Engrenagem(Vector3 posicao, Vector3 rotacaoAplicada, int numeroDeDentes, float raioMenor, float raioMaior);
    void GerarVertices();
    void aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia);

private:
    void DrawEngrenagem();

};

#endif // ENGRENAGEM_H