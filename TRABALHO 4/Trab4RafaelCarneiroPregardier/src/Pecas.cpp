#include "Pecas.h"

/**
 * Construtor da classe Pecas
 * 
 * @param posicao Vector3
 * @param rotacaoAplicada Vector3
 * 
 */
Pecas::Pecas(Vector3 posicao, Vector3 rotacaoAplicada){
    this->posicao = posicao;
    this->rotacaoAplicada = rotacaoAplicada;
}

/**
 * Aplica a pipeline de transformações
 * 
 * @param vertices3D std::vector<Vector3>
 * @param posicaoCamera Vector3
 * @param rotacaoCamera Vector3
 * @param distancia float
 * 
 * @return void
 * 
 */
void Pecas::Pipeline(std::vector<Vector3> vertices3D, Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    std::vector<Vector2> vertices2D;

    for(size_t i = 0; i < vertices3D.size(); i++){
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

/**
 * Aplica a rotação do modelo
 * 
 * @param velocidadeRotacao float
 * 
 * @return void
 * 
 */
void Pecas::aplicarRotacaoDoModelo(float velocidadeRotacao){
    rotacaoAplicada.z -= velocidadeRotacao;
}

/**
 * Aplica a pipeline para desenhar
 * 
 * @param posicaoCamera Vector3
 * @param rotacaoCamera Vector3
 * @param distancia float
 * 
 * @return void
 * 
 */
void Pecas::aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia){
    Pipeline(vertices3D, posicaoCamera, rotacaoCamera, distancia);
}

/**
 * Aplica a rotação
 * 
 * @param ponto Vector3
 * @param angulo_x float
 * @param angulo_y float
 * @param angulo_z float
 * 
 * @return Vector3
 * 
 */
Vector3 Pecas::Rotacao(Vector3 ponto, float angulo_x, float angulo_y, float angulo_z){
    float x = ponto.x;
    float y = ponto.y;
    float z = ponto.z;

    ponto.y = y * cos(angulo_x) - z * sin(angulo_x);
    ponto.z = y * sin(angulo_x) + z * cos(angulo_x);

    y = ponto.y;
    z = ponto.z;

    ponto.x = x * cos(angulo_y) + z * sin(angulo_y);
    ponto.z = -x * sin(angulo_y) + z * cos(angulo_y);

    x = ponto.x;
    z = ponto.z;

    ponto.x = x * cos(angulo_z) - y * sin(angulo_z);
    ponto.y = x * sin(angulo_z) + y * cos(angulo_z);

    return ponto;
}

/**
 * Aplica a transformação do mundo para a câmera
 * 
 * @param ponto Vector3
 * @param posicaoCamera Vector3
 * @param rotacaoCamera Vector3
 * 
 * @return Vector3
 * 
 */
Vector3 Pecas::WorldToCamera(Vector3 ponto, Vector3 posicaoCamera, Vector3 rotacaoCamera){
    ponto.x -= posicaoCamera.x;
    ponto.y -= posicaoCamera.y;
    ponto.z -= posicaoCamera.z;

    ponto = Rotacao(ponto, rotacaoCamera.x, rotacaoCamera.y, rotacaoCamera.z);

    return ponto;
}

/**
 * Aplica a projeção
 * 
 * @param ponto Vector3
 * @param distancia float
 * 
 * @return Vector2
 * 
 */
Vector2 Pecas::Projecao(Vector3 ponto, float distancia){
    float x = ponto.x * distancia / ponto.z;
    float y = ponto.y * distancia / ponto.z;

    return Vector2{x, y};
}

/**
 * Aplica a translação
 * 
 * @param ponto Vector3
 * @param z float
 * 
 * @return Vector3
 * 
 */
Vector3 Pecas::Translacao(Vector3 ponto, float z){
    ponto.z += z;

    return ponto;
}