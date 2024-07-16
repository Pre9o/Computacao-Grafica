#include "gl_canvas2d.h"

#include <math.h>
#include <vector>

#define PI 3.14159265359

class Modelos {
    public:
        std::vector<Vector3> vertices3D;
        std::vector<Vector2> vertices2D;

        Vector3 coordenadasIniciais;
        Vector3 rotacaoAplicada;

        Modelos(Vector3 coordenadasIniciais, Vector3 rotacaoAplicada){
            this->coordenadasIniciais = coordenadasIniciais;
            this->rotacaoAplicada = rotacaoAplicada;
        }

        void Pipeline(std::vector<Vector3> vertices3D, Vector3 cameraPosition, Vector3 cameraRotation, float distance){
            std::vector<Vector2> vertices2D;

            for(size_t i = 0; i < vertices3D.size(); i++){
                Vector3 ponto = vertices3D[i];
                ponto = Rotacao(ponto, rotacaoAplicada.x, rotacaoAplicada.y, rotacaoAplicada.z);
                
                ponto = ponto + coordenadasIniciais;

                ponto = WorldToCamera(ponto, cameraPosition, cameraRotation);

                ponto = Translacao(ponto, distance);

                Vector2 ponto2D = Projecao(ponto, distance);

                vertices2D.push_back(ponto2D);

                this->vertices2D = vertices2D;

            }
        }        

        void aplicarRotacaoDoModelo(float velocidadeRotacao){
            rotacaoAplicada.z -= velocidadeRotacao;
        }

        virtual void aplicarPipelineParaDesenho(Vector3 cameraPosition, Vector3 cameraRotation, float distance){
        }

    private:
        Vector3 Rotacao(Vector3 ponto, float angulo_x, float angulo_y, float angulo_z){
            float x = ponto.x;
            float y = ponto.y;
            float z = ponto.z;

            // Rotação em torno do eixo X
            ponto.y = y * cos(angulo_x) - z * sin(angulo_x);
            ponto.z = y * sin(angulo_x) + z * cos(angulo_x);

            y = ponto.y;
            z = ponto.z;

            // Rotação em torno do eixo Y
            ponto.x = x * cos(angulo_y) + z * sin(angulo_y);
            ponto.z = -x * sin(angulo_y) + z * cos(angulo_y);

            x = ponto.x;
            z = ponto.z;

            // Rotação em torno do eixo Z
            ponto.x = x * cos(angulo_z) - y * sin(angulo_z);
            ponto.y = x * sin(angulo_z) + y * cos(angulo_z);


            return ponto;
        }

        Vector3 WorldToCamera(Vector3 ponto, Vector3 cameraPosition, Vector3 cameraRotation){
            // Translação da câmera
            ponto.x -= cameraPosition.x;
            ponto.y -= cameraPosition.y;
            ponto.z -= cameraPosition.z;

            // Rotação da câmera
            ponto = Rotacao(ponto, cameraRotation.x, cameraRotation.y, cameraRotation.z);

            return ponto;
        }

        Vector2 Projecao(Vector3 ponto, float distance){
            float x;
            float y;

            x = ponto.x * distance / ponto.z;
            y = ponto.y * distance / ponto.z;

            return Vector2(x, y);
        }

        Vector3 Translacao(Vector3 ponto, float z){
            ponto.z += z;

            return ponto;
        }
};