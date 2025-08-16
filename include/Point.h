#pragma once
#include "Thing.h"
#include <iostream>

class Point : public Thing {
private:
    double x, y, z;

public:
    // Construtores
    Point() : Thing("Point"), x(0), y(0), z(0) {}
    Point(double x, double y, double z) : Thing("Point"), x(x), y(y), z(z) {}

    // Print no formato Point(x, y, z)
    inline void print() const {
        std::cout << "Point(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Getters
    inline const double& getX() const { return x; }
    inline const double& getY() const { return y; }
    inline const double& getZ() const { return z; }

};
