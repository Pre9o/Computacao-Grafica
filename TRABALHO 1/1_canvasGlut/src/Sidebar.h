#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Botao.h"
#include "Image.h"
#include "ImagesFunctions.h"
#include "ImageManager.h"

class Sidebar {
public:
    std::vector<Botao*> botoes;

    void ConstruirBotoes(ImageManager &imageManager);
};

#endif // SIDEBAR_H
