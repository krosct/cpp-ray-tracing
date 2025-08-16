#pragma once
#include "Point.h"
#include "Vector.h"

class Ray {
public:
    Point origin; // Origem do raio
    Vector direction; // Direção do raio

    Ray(const Point& a, const Vector& b) : origin(a), direction(b) {}
};