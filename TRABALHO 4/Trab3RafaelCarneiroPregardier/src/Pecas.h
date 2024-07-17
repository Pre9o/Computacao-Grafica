#ifndef PECAS_H
#define PECAS_H

#include "gl_canvas2d.h"

#include <math.h>
#include <vector>

class Pecas {
public:
    std::vector<Vector3> vertices3D;
    std::vector<Vector2> vertices2D;

    Vector3 posicao;
    Vector3 rotacaoAplicada;

    Pecas(Vector3 posicao, Vector3 rotacaoAplicada);

    void Pipeline(std::vector<Vector3> vertices3D, Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia);
    void aplicarRotacaoDoModelo(float velocidadeRotacao);
    virtual void aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia);

private:
    Vector3 Rotacao(Vector3 ponto, float angulo_x, float angulo_y, float angulo_z);
    Vector3 WorldToCamera(Vector3 ponto, Vector3 posicaoCamera, Vector3 rotacaoCamera);
    Vector2 Projecao(Vector3 ponto, float distancia);
    Vector3 Translacao(Vector3 ponto, float z);
};

#endif // PECAS_H