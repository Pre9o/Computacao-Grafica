#include "gl_canvas2d.h"
#include "Vector2.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Tabuleiro {
    public:
    std::vector<Vector2> extremos_tabuleiro;
    int matriz_tabuleiro[10][7];
    int matriz_tabuleiro_aux[10][7];

    Tabuleiro() {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 7; j++) {
                matriz_tabuleiro[i][j] = 0;
                matriz_tabuleiro_aux[i][j] = 0;
            }
        }
    }

    void desenhaTabuleiro() {
        for(int i = 0; i < extremos_tabuleiro.size(); i++){
            CV::color(1, 1, 0);
		    CV::line(extremos_tabuleiro[i], extremos_tabuleiro[(i + 1) % 4]);
	    }
    }

    void setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
        extremos_tabuleiro.push_back(p1);
        extremos_tabuleiro.push_back(p2);
        extremos_tabuleiro.push_back(p3);
        extremos_tabuleiro.push_back(p4);

        printf("%d\n", extremos_tabuleiro.size());
    }
};