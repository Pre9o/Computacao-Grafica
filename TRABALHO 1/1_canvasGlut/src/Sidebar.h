#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Botao.h"

//criar uma classe para renderizar a barra lateral com os botoes que manipulam as imagens.
class Sidebar {
public:
    int x, y, width, height;
    EstiloBotao estiloBotao;
    std::vector<Botao*> botoes;
    
    Sidebar(int _x, int _y, int _width, int _height, EstiloBotao _estiloBotao) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        estiloBotao = _estiloBotao;
    }
    
    void addBotao(Botao* botao) {
        botoes.push_back(botao);
    }
    
    void render() {
        CV::color(0.5, 0.5, 0.5);
        CV::rectFill(x, y, x + width, y + height);
    
        for (int i = 0; i < botoes.size(); i++) {
            botoes[i]->Render();
        }
    }
    
    Botao* colidiu(int mx, int my) {
        for (int i = 0; i < botoes.size(); i++) {
            if (botoes[i]->Colidiu(mx, my)) {
                return botoes[i];
            }
        }
        return NULL;
    }
    };