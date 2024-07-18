#ifndef ENGRENAGEM_H
#define ENGRENAGEM_H

#include "Pecas.h" 

/**
 * @brief Classe responsável criar e desenhar uma engrenagem
 * 
 * A classe Engrenagem é responsável por criar e desenhar uma engrenagem. Ela herda da classe Pecas e contém métodos para desenhar a engrenagem e gerar os vértices da engrenagem.
 * 
 */
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