#pragma once

#include <cmath>
#include <vector>
#include "Vector.h"

class Matrix {
    public:
    std::vector<std::vector<double>> matrixArray;
    size_t height;
    size_t width;

    Matrix(int height, int width);
    Matrix(int order);

    void buildScale(double sx, double sy, double sz);
    void buildReflexion(double sx, double sy, double sz);
    void buildRotation(double angle);
    void buildRotationX(double angle);
    void buildRotationY(double angle);
    void buildRotationZ(double angle);
    void buildTranslation(Vector t);
    void buildDiagonal(std::vector<double> valuesList);
};