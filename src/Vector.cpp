// #include "../include/Vector.h"
// #include <cmath>

// using namespace std;

// // Construtores
// Vector::Vector() : x(0), y(0), z(0), Thing("Vector") {}
// Vector::Vector(double x, double y, double z) : x(x), y(y), z(z), Thing("Vector") {}

// // Print do vetor no formato Vector<x, y, z>
// void Vector::print() const {
//     std::cout << "Vector<" << x << ", " << y << ", " << z << ">" << std::endl;
// }

// // Getters
// const double& Vector::getX() const { return x; }
// const double& Vector::getY() const { return y; }
// const double& Vector::getZ() const { return z; }

// void Vector::setX(double newX) {
//     x = newX;
// }
// void Vector::setY(double newY) {
//     y = newY;
// }
// void Vector::setZ(double newZ) {
//     z = newZ;
// }

// // Calcula a magnitude (comprimento) do vetor
// double Vector::magnitude() const {
//     return sqrt(x * x + y * y + z * z);
// }

// // Retorna uma versão normalizada (comprimento 1) do vetor
// const Vector Vector::normalized() const {
//     double mag = magnitude();
//     if (mag == 0) return Vector(0, 0, 0); // Evita divisão por zero
//     return Vector(this->getX()/mag, this->getY()/mag, this->getZ()/mag);
// }
