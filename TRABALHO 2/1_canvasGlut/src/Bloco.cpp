#include "Bloco.h"

/**
 * Construtor da classe Bloco.
 * 
 */
Bloco::Bloco(){
    tamanho = Vector2(80, 80);
    cor = 0;
    this->ativo = false;
    pontos = 0;
}

/**
 * Sobrecarga do operador de comparação.
 * 
 * @param outro O bloco a ser comparado.
 * @return Retorna true se os blocos forem iguais, caso contrário retorna false.
 */
bool Bloco::operator==(const Bloco& outro) const {
    return extremos_bloco[0].x == outro.extremos_bloco[0].x && extremos_bloco[0].y == outro.extremos_bloco[0].y;
}

/**
 * Define os extremos do bloco.
 * 
 * @param extremos Os extremos do bloco.
 */
void Bloco::setExtremosBloco(std::vector<Vector2> extremos) {
    extremos_bloco = extremos;
}

/**
 * Define os atributos do bloco.
 * 
 * @param i O índice do bloco atual.
 * @param pontos A quantidade de pontos do bloco.
 * @param blocos_ativos O vetor de blocos ativos.
 * @param blocosIniciaisMaximosDoNivel O número máximo de blocos iniciais do nível.
 * @param chanceDeSerAtivo A chance de o bloco ser ativado, em porcentagem.
*/
void Bloco::setBloco(int i, int pontos, std::vector<Bloco>& blocos_ativos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo){
    cor = rand() % 20;
    this->ativo = this->definirBlocoAtivo(i, blocos_ativos, blocosIniciaisMaximosDoNivel, chanceDeSerAtivo);
    this->pontos = pontos;
}

/**
 * Define se o bloco atual será ativado ou não, com base em uma probabilidade.
 * 
 * @param i O índice do bloco atual.
 * @param blocos_ativos O vetor de blocos ativos.
 * @param blocosIniciaisMaximosDoNivel O número máximo de blocos iniciais do nível.
 * @param chanceDeSerAtivo A chance de o bloco ser ativado, em porcentagem.
 * @return Retorna true se o bloco for ativado e adicionado ao vetor de blocos ativos, caso contrário retorna false.
 */
bool Bloco::definirBlocoAtivo(int i, std::vector<Bloco>& blocos_ativos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo){
    if(i == 7 && blocos_ativos.size() < size_t(blocosIniciaisMaximosDoNivel)){
        if(rand() % 100 < chanceDeSerAtivo){
            blocos_ativos.push_back(*this);
            return true;
        }
        return false;
    }
    return false;
}


/*
 * Retorna a posição do bloco.
 * 
 * @return A posição do bloco.
 */
Vector2 Bloco::getPosicao(){
    return extremos_bloco[0];
}

/*
* Retorna o tamanho do bloco.
*
* @return O tamanho do bloco.
*/
Vector2 Bloco::getTamanho(){
    return tamanho;
}

/**
 * Diminui a quantidade de pontos do bloco em 1. Se a quantidade de pontos chegar a 0,
 * cria uma explosão, desativa o bloco, remove-o da lista de blocos ativos e incrementa
 * a pontuação em 10.
 * 
 * @param blocos_ativos A lista de blocos ativos.
 * @param pontuacao A pontuação atual.
 */
void Bloco::diminuirPontos(std::vector<Bloco>& blocos_ativos, int& pontuacao){
    this->pontos -= 1;
    if(this->pontos == 0){
        this->explosao = new Explosao(this->extremos_bloco[0].x + this->tamanho.x / 2, this->extremos_bloco[0].y + this->tamanho.y / 2, clock(), this->cor);
        this->ativo = false;
        auto it = std::find(blocos_ativos.begin(), blocos_ativos.end(), *this);
            blocos_ativos.erase(it);
        pontuacao += 10;
    }
}

/**
 * Desenha o bloco na tela.
 */
void Bloco::desenhaBloco(){
    for(size_t i = 0; i < extremos_bloco.size(); i++){
        CV::color(cor);
        CV::rectFill(extremos_bloco[0], extremos_bloco[1], extremos_bloco[2], extremos_bloco[3]);
        CV::color(1, 1, 1);
        CV::color(1, 1, 1);
        CV::text(extremos_bloco[0].x - (CV::getTextWidth(pontos, GLUT_BITMAP_HELVETICA_18)/2) + (tamanho.y/2),
                extremos_bloco[0].y + (tamanho.x/10) + (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
                pontos,
                GLUT_BITMAP_HELVETICA_18);
    }
}