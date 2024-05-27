#include "Bloco.h"

Bloco::Bloco(){
    tamanho = Vector2(80, 80); //80x80
    cor = 0;
    this->ativo = false;
    pontos = 0;
}

bool Bloco::operator==(const Bloco& outro) const {
    return extremos_bloco[0].x == outro.extremos_bloco[0].x && extremos_bloco[0].y == outro.extremos_bloco[0].y;
}

void Bloco::setExtremosBloco(std::vector<Vector2> extremos) {
    extremos_bloco = extremos;
}

void Bloco::setBloco(int i, int pontos, std::vector<Bloco>& blocos_ativos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo){
    cor = rand() % 20;
    this->ativo = this->definirBlocoAtivo(i, blocos_ativos, blocosIniciaisMaximosDoNivel, chanceDeSerAtivo);
    this->pontos = pontos;
}

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

Vector2 Bloco::getPosicao(){
    return extremos_bloco[0];
}

Vector2 Bloco::getTamanho(){
    return tamanho;
}

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