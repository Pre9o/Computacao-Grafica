#include "Tabuleiro.h"

Tabuleiro::Tabuleiro(){
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 + 400));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 260, screenHeight/2 + 401));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 261, screenHeight/2 - 400));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 - 400));

    this->setTabuleiro();
}

void Tabuleiro::setTabuleiro() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 7; j++){
            Vector2 p1 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80));
            Vector2 p2 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80));
            Vector2 p3 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80) + 80);
            Vector2 p4 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80) + 80);
            matriz_tabuleiro[i][j].setExtremosBloco({p1, p2, p3, p4});
        }
    }
}

int Tabuleiro::verificaBlocosAtivos(){
    int blocos_ativos = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 7; j++){
            if(matriz_tabuleiro[i][j].ativo == true){
                blocos_ativos++;
            }
        }
    }
    return blocos_ativos;
}

void Tabuleiro::definirBlocos(int pontos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo){
    for(int i = 0; i < 10; i++){
        chanceDeSerAtivo = 7;
        for(int j = 0; j < 7; j++){
            if(matriz_tabuleiro[i][j].ativo == false){
                matriz_tabuleiro[i][j].setBloco(i, pontos, blocos_ativos, blocosIniciaisMaximosDoNivel, chanceDeSerAtivo);
            }
            if(matriz_tabuleiro[i][j].ativo == false){
                chanceDeSerAtivo += 10;
            }
        }
    }
}

void Tabuleiro::desenhaTabuleiro() {
    for(size_t i = 0; i < extremos_tabuleiro.size(); i++){
        CV::color(1, 1, 1);
        CV::line(extremos_tabuleiro[i], extremos_tabuleiro[(i + 1) % 4]);
    }
}

void Tabuleiro::reorganizaTabuleiro(int pontos, int blocosIniciaisMaximosDoNivel){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++){
            matriz_tabuleiro[i][j] = matriz_tabuleiro[i+1][j];
        }
    }
    for(int j = 0; j < 7; j++){
        matriz_tabuleiro[9][j].setBloco(9, pontos, blocos_ativos, blocosIniciaisMaximosDoNivel, 0);
    }

    definirBlocos(pontos, blocosIniciaisMaximosDoNivel, 7);

}

void Tabuleiro::setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
    extremos_tabuleiro[0] = p1;
    extremos_tabuleiro[1] = p2;
    extremos_tabuleiro[2] = p3;
    extremos_tabuleiro[3] = p4;
}