#include "Engrenagem.h"

/**
 * Construtor da classe Engrenagem
 *  
 * @param posicao Vector3
 * @param rotacaoAplicada Vector3
 * @param numeroDeDentes int
 * @param raioMenor float
 * @param raioMaior float
 * 
 */
Engrenagem::Engrenagem(Vector3 posicao, Vector3 rotacaoAplicada, int numeroDeDentes, float raioMenor, float raioMaior) : Pecas(posicao, rotacaoAplicada){
    this->numeroDeDentes = numeroDeDentes;
    this->raioMenor = raioMenor;
    this->raioMaior = raioMaior;
}

/**
 * Desenha a engrenagem
 * 
 * @return void
 * 
 */
void Engrenagem:: DrawEngrenagem(){
    for(int i = 0; i < numeroDeDentes; i++){
        CV::color(1, 1, 1);
        CV::line(vertices2D[i].x, vertices2D[i].y, vertices2D[(i + 1) % numeroDeDentes].x, vertices2D[(i + 1) % numeroDeDentes].y);
        CV::line(vertices2D[i + numeroDeDentes].x, vertices2D[i + numeroDeDentes].y, vertices2D[((i + 1) % numeroDeDentes) + numeroDeDentes].x, vertices2D[((i + 1) % numeroDeDentes) + numeroDeDentes].y);
        CV::line(vertices2D[i].x, vertices2D[i].y, vertices2D[i + numeroDeDentes].x, vertices2D[i + numeroDeDentes].y);
    }

    vertices2D.clear();
    vertices3D.clear();
}

/**
 * Gera os vértices da engrenagem
 * 
 * @return void
 * 
 */
void Engrenagem::GerarVertices(){
    float a = 0;
    float raioAtual = raioMenor;
    int i = 0;

    float passo = 2 * PI / numeroDeDentes;

    for (int i = 0; i < numeroDeDentes; i++){
        vertices3D.push_back(Vector3(0, 0, 0));
        vertices3D.push_back(Vector3(0, 0, 0));
    }

    for (a = 0, i = 0; a <= 2 * PI; a += passo, i += 2){
        float x = raioAtual * cos(a);
        float y = raioAtual * sin(a);

        a += passo;
        
        float x2 = raioAtual * cos(a);
        float y2 = raioAtual * sin(a);

        raioAtual = (raioAtual == raioMenor) ? raioMaior : raioMenor;

        vertices3D[i] = Vector3(x, y, 0);
        vertices3D[i + 1] = Vector3(x2, y2, 0);
    }

    int tamanhoEngrenagem = 100;

    for (int i = 0; i <= numeroDeDentes; i++){
        vertices3D[i + numeroDeDentes] = Vector3(vertices3D[i].x, vertices3D[i].y, tamanhoEngrenagem);
    }
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
void Engrenagem::aplicarPipelineParaDesenho(Vector3 posicaoCamera, Vector3 rotacaoCamera, float distancia) {
    GerarVertices();
    Pipeline(vertices3D, posicaoCamera, rotacaoCamera, distancia);
    DrawEngrenagem();
}