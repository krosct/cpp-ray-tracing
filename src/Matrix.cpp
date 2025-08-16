#include "../include/Matrix.h"
#include "../include/Operations.h"
#include <cmath>
#include <iostream>

Matrix::Matrix(int order) : height(order), width(order) { 
    matrixArray = std::vector<std::vector<double>>(height, std::vector<double>(width));
}

Matrix::Matrix(int height, int width) : height(height), width(width) { 
    matrixArray = std::vector<std::vector<double>>(height, std::vector<double>(width));
}

// sx = sy = sz > 1: dilatação isotrópica
// sx = sy = sz < 1: contração isotrópica
// sx != sy ou sx != sz ou sz != sy: deformação ou escala anisotrópica
void Matrix::buildScale(double sx, double sy, double sz) {
    std::vector<double> a = {sx, sy, sz, 1};
    buildDiagonal(a);
}

// rx, ry e rz podem assumir 1 ou -1
// 1 sinal positivo = reflexão em torno do eixo
// 2 sinais positivos = reflexão em torno do plano
// 3 sinais negativos = reflexão em torno da origem (vetor simétrico)
void Matrix::buildReflexion(double rx, double ry, double rz) {
    std::vector<double> a = {rx, ry, rz, 1};
    buildDiagonal(a);
}

void Matrix::buildRotation(double angle) {
    // Rotação INTRINSECA
    // Se rotateZ, rotateY, rotateX rotacionam em torno dos eixos globais (fixos):
    // obj.rotateZ(angle_z): Gira em torno do eixo Z global.
    // obj.rotateY(angle_y): Gira em torno do eixo Y global.
    // obj.rotateX(angle_x): Gira em torno do eixo X global.
    // Neste caso (rotações extrínsecas), a matriz final para aplicar aos pontos 
    // seria M_X * M_Y * M_Z. Se você está aplicando-as sequencialmente, você estaria aplicando M_Z primeiro, depois M_Y, depois M_X.
    // Se rotateZ, rotateY, rotateX rotacionam em torno dos eixos locais (do objeto, que se movem):
    // obj.rotateZ(angle_z): Gira em torno do eixo Z local do objeto.
    // obj.rotateY(angle_y): Gira em torno do novo eixo Y local (que se moveu devido à rotação Z).
    // obj.rotateX(angle_x): Gira em torno do novo eixo X local (que se moveu devido às rotações Z e Y).
    // Neste caso (rotações intrínsecas), a matriz final para aplicar aos pontos é M_X * M_Y * M_Z 
    // (onde a multiplicação é da direita para a esquerda, significando que M_Z é aplicada "primeiro").
    
    if (height == width && height == 4) {
        Matrix rotX(4);
        Matrix rotY(4);
        Matrix rotZ(4);
    
        rotX.buildRotationX(angle);
        rotY.buildRotationY(angle);
        rotZ.buildRotationZ(angle);
    
        // Pitch (Z) -> Yaw (Y) -> Roll (X)
        // Olhar para cima/baixo (Pitch): Isso geralmente envolve girar em torno do eixo Z (o eixo horizontal, 
        // Olhar para os lados (Yaw): Isso geralmente envolve girar em torno do eixo Y (o eixo vertical).
        // que cruza a tela).
        // Inclinar a cabeça (Roll): Isso geralmente envolve girar em torno do eixo X (o eixo de profundidade, 
        // que "sai" da sua visão).
        // Ordem da multiplicação de matrizes para rotações Euler.
        // Por exemplo, para ZYX intrínseco (roll X, pitch Y, yaw Z)
        // ou XYZ extrínseco (X, Y, Z em eixos fixos).
        // A ordem é rotX * (rotY * rotZ) para essa composição.
        // Matrix result = rotX * rotY * rotZ;
        Matrix result = (rotX * (rotY * rotZ));
        matrixArray = result.matrixArray;
        height = result.height;
        width = result.width;
    }
}

void Matrix::buildRotationZ(double angle) {
    if (height == width && height == 4) {
        buildDiagonal({1,1,1,1});
        matrixArray[0][0] = cos(angle);
        matrixArray[0][1] = -sin(angle);
        matrixArray[1][0] = sin(angle);
        matrixArray[1][1] = cos(angle);
    }
}

void Matrix::buildRotationX(double angle) {
    if (height == width && height == 4) {
        buildDiagonal({1,1,1,1});
        matrixArray[1][1] = cos(angle);
        matrixArray[1][2] = -sin(angle);
        matrixArray[2][1] = sin(angle);
        matrixArray[2][2] = cos(angle);
    }
}

void Matrix::buildRotationY(double angle) {
    if (height == width && height == 4) {
        buildDiagonal({1,1,1,1});
        matrixArray[0][0] = cos(angle);
        matrixArray[0][2] = sin(angle);
        matrixArray[2][0] = -sin(angle);
        matrixArray[2][2] = cos(angle);
    }
}

void Matrix::buildTranslation(Vector t) {
    buildDiagonal({1,1,1,1});

    matrixArray[0][width-1] = t.getX();
    matrixArray[1][width-1] = t.getY();
    matrixArray[2][width-1] = t.getZ();
}

void Matrix::buildDiagonal (std::vector<double> valuesList) {
    if (height != width) {
        std::cout << "ERROR: MATRIZ NÃO QUADRADA NÃO POSSUI DIAGONAL!" << std::endl;
        return;
    }
    
    if (valuesList.size() != height) {
        std::cout << "ERROR: TAMANHO DO VETOR DIAGONAL DIFERENTE DA ORDEM DA MATRIZ!" << std::endl;
        return;
    }

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            matrixArray[i][j] = 0;
        }
    }

    for (size_t i = 0; i < height; ++i) {
        matrixArray[i][i] = valuesList[i];
    }
}