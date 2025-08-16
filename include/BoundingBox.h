#pragma once
#include "../include/Point.h"
#include "../include/Ray.h"
#include "UniversalVars.h"

class BoundingBox {
public:
    // Pontos diagonais superior e inferior
    Point min, max;

    BoundingBox() : min(maxDouble, maxDouble, maxDouble), max(-maxDouble, -maxDouble, -maxDouble) {}
    BoundingBox(const Point& min, const Point& max) : min(min), max(max) {}

    // Verifica se um raio atinge a caixa
    bool hit(const Ray& r) const;
    // Aumenta a caixa para incluir uma outra caixa
    inline void expand(const BoundingBox& other);
    // Aumenta a caixa para incluir um ponto
    inline void expand(const Point& p);
    bool partialInside(const BoundingBox& other) const;
};