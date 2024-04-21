#include <vector>
#include <algorithm>
#include <string>
#include "gl_canvas2d.h"

void ManipularVetorImagem(std::vector<Bmp*> &images, Bmp* &image);

void ArrastarImagem(Bmp* image, int x, int y);

void DesenharMoldura(Bmp* image);

void DrawImage(Bmp* image);

void InicializarParametros(Bmp* image, int offset);

void LoadImages(std::vector<Bmp*> &images, const char* path);

void Draw4thImage(Bmp* image);

void DesenharHistogramaRed(Bmp* image);
void DesenharHistogramaGreen(Bmp* image);
void DesenharHistogramaBlue(Bmp* image);
void DesenharHistogramaGray(Bmp* image);

void rotateImage(Bmp* image, double angle);

void desenharDigito(int digito, int posX, int posY);
void desenharTempo(int horas, int minutos, int segundos);
