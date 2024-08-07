/*
A classe Sidebar representa a barra lateral da interface do usuário que contém botões para manipular as imagens. Cada botão é representado por uma instância da classe Botao e tem uma ação associada que é executada quando o botão é clicado.

Métodos:
- ConstruirBotoes(ImageManager &imageManager): Constrói os botões na barra lateral. Cada botão tem uma posição, tamanho, cor e ação associada. As ações dos botões incluem carregar imagens, aplicar várias operações às imagens (como flip vertical/horizontal, grayscale, red, green, blue) e visualizar o histograma da imagem selecionada.
*/

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Botao.h"
#include "Image.h"
#include "ImagesFunctions.h"
#include "ImageManager.h"
#include <time.h>

class Sidebar {
public:
    // Vetor para armazenar os botões do menu principal
    std::vector<Botao*> botoesMenuInicial;
    std::vector<Botao*> botoesMenuPausa;

    // Método para construir os botões do menu principal
    void ConstruirBotoesMenuInicial(int *opcaoMenu, bool *setarUsername){
        // Se a barra lateral ainda não tem botões, cria os botões
        if(botoesMenuInicial.size() == 0){
            botoesMenuInicial.push_back(new Botao(-100, 0, 200, 50, "New Game", 1, 1, 1, [opcaoMenu](){
                printf("Botão Play clicado\n");
                *opcaoMenu = 1;
            }));
            botoesMenuInicial.push_back(new Botao(-100, -100, 200, 50, "Continue", 1, 1, 1, [opcaoMenu](){
                printf("Botão Settings clicado\n");
                *opcaoMenu = 1;
            }));
            botoesMenuInicial.push_back(new Botao(-100, -200, 200, 50, "Choose Username", 1, 1, 1, [opcaoMenu, setarUsername](){
                printf("Botão Username clicado\n");
                *opcaoMenu = 3;
                *setarUsername = true;
            }));
            botoesMenuInicial.push_back(new Botao(-100, -300, 200, 50, "Quit", 1, 1, 1, [](){
                printf("Botão Quit clicado\n");
                exit(0);
            }));
        }
    }

    void ConstruirBotoesMenuPausa(int *opcaoMenu){
        // Se a barra lateral ainda não tem botões, cria os botões
        if(botoesMenuPausa.size() == 0){
            botoesMenuPausa.push_back(new Botao(-100, 0, 200, 50, "Resume", 1, 1, 1, [opcaoMenu](){
                printf("Botão Resume clicado\n");
                *opcaoMenu = 1;
            }));
            botoesMenuPausa.push_back(new Botao(-100, -100, 200, 50, "Settings", 1, 1, 1, [](){
                printf("Botão Settings clicado\n");
            }));
            botoesMenuPausa.push_back(new Botao(-100, -200, 200, 50, "Main Menu", 1, 1, 1, [opcaoMenu](){
                printf("Botão Quit clicado\n");
                *opcaoMenu = 0;
            }));
        }
    }
};

#endif // SIDEBAR_H