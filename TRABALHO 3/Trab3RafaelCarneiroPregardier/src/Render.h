// render.h
#pragma once

#include "Sidebar.h"
#include "Controle.h"

void renderizarMenuInicial(Sidebar sidebar, ImageManager imageManagerMenuInicial, int screenWidth, int screenHeight);
void renderizarJogo(Controle& controle, bool& firstMove, clock_t& lastTime, bool& carregado);
void renderizarMenuPausa(Sidebar sidebar, ImageManager imageManagerMenuPausa, ImageManager imageManagerMenuInicial, int screenWidth, int screenHeight);