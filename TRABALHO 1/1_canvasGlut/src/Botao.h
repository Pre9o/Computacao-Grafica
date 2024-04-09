#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"
#include <functional>

class Botao{
  float altura, largura, x, y;
  char label[100];
  float r_color;
  float g_color;
  float b_color;

public:
  std::function<void()> onClick;

  Botao(float _x, float _y, float _larg, float _alt, const char* _label, float _r_color, float _g_color, float _b_color, std::function<void()> _onClick)
  {
    //X = ponto inicial X
    //Y = ponto inicial Y
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     strcpy(label, _label);
     onClick = _onClick;
      r_color = _r_color;
      g_color = _g_color;
      b_color = _b_color;
  }


  void Render()
  {
      CV::color(r_color, g_color, b_color);
      CV::rectFill(x, y, x + largura, y + altura);
      CV::color(0, 0, 0);
      CV::text(x - (CV::getTextWidth(label, GLUT_BITMAP_HELVETICA_18)/2) + (largura/2),
               y + (altura/10) + (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
               label,
               GLUT_BITMAP_HELVETICA_18);
  }

  bool Colidiu(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          return true;
      }
      return false;
  }
};

#endif
