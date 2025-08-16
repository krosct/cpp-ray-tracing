#include "../include/BoundingBox.h"

// Interseção raio-caixa (algoritmo slab test)
bool BoundingBox::hit(const Ray& r) const {
    // Equação da reta: p(t) = pr + v * t, onde
    // p(t) é o ponto que queremos descobrir, pr é um ponto da reta,
    // v é o vetor diretor da reta e t é uma escalar
    // t = (p(t) - pr) / v

    // Calcula onde o raio entra e sai dos planos X da caixa
    double txmin = (min.getX() - r.origin.getX()) / r.direction.getX();
    double txmax = (max.getX() - r.origin.getX()) / r.direction.getX();

    // Dependendo de onde o raio partir pode ser que o txmin e txmax 
    // estejam invertidos, então corrigimos aqui para que txmin seja sempre o ponto de "entrada"
    if (txmin > txmax) std::swap(txmin, txmax);

    double tymin = (min.getY() - r.origin.getY()) / r.direction.getY();
    double tymax = (max.getY() - r.origin.getY()) / r.direction.getY();

    if (tymin > tymax) std::swap(tymin, tymax);

    // em 11s, a bola azul é o tymax e a bola rosa a esquerda é o txmin
    if ((txmin > tymax) || (tymin > txmax)) return false;

    // Atualiza os valores de tx para quando de fato ocorre a intersecção
    // Ponto de entrada final é o mais distante (o maior)
    if (tymin > txmin) txmin = tymin;
    // Ponto de saída final é o mais próximo (o menor)
    if (tymax < txmax) txmax = tymax;

    // Repete a mesma lógica para os planos Z
    double tzmin = (min.getZ() - r.origin.getZ()) / r.direction.getZ();
    double tzmax = (max.getZ() - r.origin.getZ()) / r.direction.getZ();

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((txmin > tzmax) || (tzmin > txmax)) return false;

    return true;
}

// Expande a caixa para incluir outra caixa
void BoundingBox::expand(const BoundingBox& other) {
    // Pega o menor ponto entre os dois 'min' e o maior ponto entre os dois 'max'
    min = Point(std::min(min.getX(), other.min.getX()), std::min(min.getY(), other.min.getY()), std::min(min.getZ(), other.min.getZ()));
    max = Point(std::max(max.getX(), other.max.getX()), std::max(max.getY(), other.max.getY()), std::max(max.getZ(), other.max.getZ()));
}

// Expande a caixa para incluir um ponto
void BoundingBox::expand(const Point& p) {
    // Compara as coordenadas do ponto 'p' com as da caixa atual para ver se precisa crescer
    min = Point(std::min(min.getX(), p.getX()), std::min(min.getY(), p.getY()), std::min(min.getZ(), p.getZ()));
    max = Point(std::max(max.getX(), p.getX()), std::max(max.getY(), p.getY()), std::max(max.getZ(), p.getZ()));
}

bool BoundingBox::partialInside(const BoundingBox& other) const {
    // Verifica se não há sobreposição em nenhum eixo.
    if (max.getX() < other.min.getX() || min.getX() > other.max.getX()) return false;
    if (max.getY() < other.min.getY() || min.getY() > other.max.getY()) return false;
    if (max.getZ() < other.min.getZ() || min.getZ() > other.max.getZ()) return false;

    return true; // Há sobreposição
}