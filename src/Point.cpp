#ifndef POINTHEADER
#define POINTHEADER
#include <iostream>

/*
Classe de pontos.

A saber que:
    - x = componente x
    - y = componente y
    - z = componente z

A classe precisa ser instanciada passando as componentes x, y e z
*/

class Point{
    private:
        double x;
        double y;
        double z;
public:

    //Construtores
    Point() {}
    Point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    //Implemente os métodos de pontos aqui

    //Print do vetor no formato (x, y, z)
    void print(){
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    //Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }   
};

#endif