#pragma once
#include "Thing.h"
#include <iostream>
#include <cmath>

class Vector : public Thing {
private:
    double x, y, z;

public:
    // Construtores
    Vector() : Thing("Vector"), x(0), y(0), z(0) {}
    Vector(double x, double y, double z) : Thing("Vector"), x(x), y(y), z(z) {}

    // Print do vetor no formato Vector<x, y, z>
    inline void print() const {
        std::cout << "Vector<" << x << ", " << y << ", " << z << ">" << std::endl;
    }

    // Getters
    inline const double& getX() const { return x; }
    inline const double& getY() const { return y; }
    inline const double& getZ() const { return z; }

    // Setters
    inline void setX(double newX) { x = newX; }
    inline void setY(double newY) { y = newY; }
    inline void setZ(double newZ) { z = newZ; }

    // Calcula a magnitude (comprimento) do vetor
    inline double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    // Retorna uma versão normalizada (comprimento 1) do vetor
    inline const Vector normalized() const {
        double mag = magnitude();
        if (mag == 0) { return Vector(0, 0, 0); }; // Evita divisão por zero
        return Vector(getX()/mag, getY()/mag, getZ()/mag);
    }
};
