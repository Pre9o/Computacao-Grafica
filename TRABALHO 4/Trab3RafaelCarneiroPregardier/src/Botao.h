#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"
#include <functional>

/**
 * @brief Classe que representa um botão.
 */
class Botao{
  float altura, largura, x, y;
  char label[100];
  float r_color;
  float g_color;
  float b_color;
  float x_render, y_render;

public:
  std::function<void()> onClick;

  /**
   * @brief Constrói um novo objeto Botao.
   * 
   * @param _x A coordenada x do botão.
   * @param _y A coordenada y do botão.
   * @param _larg A largura do botão.
   * @param _alt A altura do botão.
   * @param _label O texto do rótulo do botão.
   * @param _r_color O componente vermelho da cor do botão.
   * @param _g_color O componente verde da cor do botão.
   * @param _b_color O componente azul da cor do botão.
   * @param _onClick A função a ser chamada quando o botão for clicado.
   */
  Botao(float _x, float _y, float _larg, float _alt, const char* _label, float _r_color, float _g_color, float _b_color, std::function<void()> _onClick)
  {
    altura  = _alt;
    largura = _larg;
    x = _x;
    y = _y;
    x_render = x;
    y_render = y;
    strcpy(label, _label);
    onClick = _onClick;
    r_color = _r_color;
    g_color = _g_color;
    b_color = _b_color;
  }

  /**
   * @brief Renderiza o botão na tela.
   */
  void Render()
  {
      CV::color(r_color, g_color, b_color);
      CV::rectFill(x_render, y_render, x_render + largura, y_render + altura);
      CV::color(0, 0, 0);
      CV::text(x_render - (CV::getTextWidth(label, GLUT_BITMAP_HELVETICA_18)/2) + (largura/2),
               y_render + (altura/10) + (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
               label,
               GLUT_BITMAP_HELVETICA_18);
  }

  /**
   * @brief Atualiza a posição do botão para ficar centralizado na tela.
   * 
   * @param metade_horizontal O centro horizontal da tela.
   * @param metade_vertical O centro vertical da tela.
   */
  void AtualizarPosicaoMeioTela(float metade_horizontal, float metade_vertical)
  {
      x_render = x + metade_horizontal;
      y_render = y + metade_vertical;
  }

  /**
   * @brief Chama a função onClick quando o botão for clicado.
   */
  void OnClick()
  {
      onClick();
  }

  /**
   * @brief Verifica se o botão foi clicado nas coordenadas fornecidas.
   * 
   * @param mx A coordenada x do clique.
   * @param my A coordenada y do clique.
   * @return true se o botão foi clicado, false caso contrário.
   */
  bool Colidiu(int mx, int my)
  {
      if( mx >= x_render && mx <= (x_render + largura) && my >= y_render && my <= (y_render + altura) )
      {
          return true;
      }
      return false;
  }
};

#endif
