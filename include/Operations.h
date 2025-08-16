#pragma once
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"
#include <iostream>

// Adição: Ponto + Vetor = Ponto
inline Point operator+(const Point& p, const Vector& v) {
    return Point(p.getX() + v.getX(), p.getY() + v.getY(), p.getZ() + v.getZ());
}

inline Point operator+(const Vector& v, const Point& p) {
    return p + v; // Reutiliza a função acima
}

// Subtração: Ponto - Vetor = Vetor
inline Point operator-(const Point& p, const Vector& v) {
    return Point(p.getX() - v.getX(), p.getY() - v.getY(), p.getZ() - v.getZ());
}

inline Point operator-(const Vector& v, const Point& p) {
    return p - v; // Reutiliza a função acima
}

// Subtração: Ponto - Ponto = Vetor
inline Vector operator-(const Point& p1, const Point& p2) {
    return Vector(p1.getX() - p2.getX(), p1.getY() - p2.getY(), p1.getZ() - p2.getZ());
}

// Adição: Vetor + Vetor = Vetor
inline Vector operator+(const Vector& v1, const Vector& v2) {
    return Vector(v1.getX() + v2.getX(), v1.getY() + v2.getY(), v1.getZ() + v2.getZ());
}

// Subtração: Vetor - Vetor = Vetor
inline Vector operator-(const Vector& v1, const Vector& v2) {
    return Vector(v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());
}

// Multiplicação: Escalar * Vetor = Vetor
inline Vector operator*(double scalar, const Vector& v) {
    return Vector(scalar * v.getX(), scalar * v.getY(), scalar * v.getZ());
}

inline Vector operator*(const Vector& v, double scalar) {
    return scalar * v; // Reutiliza a função acima
}

// Produto Vetorial entre vetores
inline Vector cross(const Vector& v1, const Vector& v2) {
    double crossX = v1.getY() * v2.getZ() - v1.getZ() * v2.getY();
    double crossY = v1.getZ() * v2.getX() - v1.getX() * v2.getZ();
    double crossZ = v1.getX() * v2.getY() - v1.getY() * v2.getX();
    return Vector(crossX, crossY, crossZ);
}

// Divisão por escalar 
inline Vector operator/(const Vector& v, double t) {
    return Vector(v.getX() / t, v.getY() / t, v.getZ() / t);
}

// Produto Escalar: Vetor * Vetor = Escalar
inline double dot(const Vector& v1, const Vector& v2) {
    return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ();
}

// Comparação entre dois pontos
inline bool operator==(const Point& a, const Point& b) {
    if (a.getX() != b.getX()) { return false; }
    if (a.getY() != b.getY()) { return false; }
    if (a.getZ() != b.getZ()) { return false; }
    return true;
}

inline bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}

// Comparação entre dois vetores
inline bool operator==(const Vector& a, const Vector& b) {
    if (a.getX() != b.getX()) { return false; }
    if (a.getY() != b.getY()) { return false; }
    if (a.getZ() != b.getZ()) { return false; }
    return true;
}

inline bool operator!=(const Vector& a, const Vector& b) {
    return !(a == b);
}

// Retorna o valor do vermelho normalizado (entre 0 e 1)
inline double red(const double& a) {
    return a/255;
}

// Retorna o valor do verde normalizado (entre 0 e 1)
inline double green(const double& a) {
    return a/255;
}

// Retorna o valor do azul normalizado (entre 0 e 1)
inline double blue(const double& a) {
    return a/255;
}

// Matriz * Vetor = Vetor
inline Vector operator*(const Matrix& m, const Vector& v) {
    double vArray[4] {v.getX(), v.getY(), v.getZ(), 1};
    double arrayResult[4] {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arrayResult[i] += m.matrixArray[i][j] * vArray[j];
        }
    }
    return Vector(arrayResult[0], arrayResult[1], arrayResult[2]);
}

// Vetor * Matriz = Vetor
inline Vector operator*(const Vector& v, const Matrix& m) {
    return m * v;
}

// Matriz * Ponto = Ponto
inline Point operator*(const Matrix& m, const Point& p) {
    double pArray[4] {p.getX(), p.getY(), p.getZ(), 1};
    double arrayResult[4] {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arrayResult[i] += m.matrixArray[i][j] * pArray[j];
        }
    }
    return Point(arrayResult[0], arrayResult[1], arrayResult[2]);
}

// Ponto * Matriz = Ponto
inline Point operator*(const Point& p, const Matrix& m) {
    return m * p;
}

// Matriz * Matriz = Matriz
inline Matrix operator*(const Matrix& m1, const Matrix& m2) {
    if (m1.width != m2.height) {
        std::cerr << "ERROR: As matrizes não podem ser multiplicadas. "
                  << "Número de colunas da primeira (" << m1.width << ") "
                  << "diferente do número de linhas da segunda (" << m2.height << ")."
                  << std::endl;
        return Matrix(0, 0);
    }

    Matrix result(m1.height, m2.width);

    for (size_t i = 0; i < m1.height; ++i) { // Linhas da matriz resultante (m1)
        for (size_t j = 0; j < m2.width; ++j) { // Colunas da matriz resultante (m2)
            for (size_t k = 0; k < m1.width; ++k) { // Colunas de m1 (linhas de m2)
                result.matrixArray[i][j] += m1.matrixArray[i][k] * m2.matrixArray[k][j];
            }
        }
    }
    result.height = m1.height;
    result.width = m2.width;
    return result;
}

// Retorna em radianos o ângulo entrado (ang)
inline double rad(double ang) {
    return (ang * M_PI) / 180;
}

// Multiplicação componente a componente de um vetor. Vetor * Vetor = Vetor
inline Vector operator*(const Vector& v1, const Vector& v2) {
    return Vector(v1.getX() * v2.getX(), v1.getY() * v2.getY(), v1.getZ() * v2.getZ());
}

// Soma de vetores com operador curto - não sei como se chama isso, hehe
inline void operator+=(Vector& a, const Vector& b) {
    a = a + b;
}

// Inverter o sinal do vetor
inline Vector operator-(const Vector& a) {
    return -1 * a;
}