#include "../include/Cube.h"
#include "../include/Triangle.h"
#include "../include/Operations.h"
#include "../include/Ray.h"
#include <iostream>
#include <vector>

using namespace std;

// Construtores
Cube::Cube(string objName, Point a, Point b, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture, Texture* heightTexture) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Cube", visualTexture, heightTexture),
    a(a),
    b(b) { resetPoints(); }

    
void Cube::addTriangle(Point a1, Point b1, Point c1) {
    triangles.push_back(Triangle("tempTriangle",a1,b1,c1,getka(),getkd(),getks(),getshininess(),getkr(),getkt(), visualTexture, heightTexture));
}

void Cube::resetPoints() {
    Vector left = a - b;
    double edgeLength = left.magnitude();
    Vector forward = cross(left, {0,1,0}).normalized();
    Vector walkToC = forward * edgeLength;
    c = b + walkToC;
    Vector up = cross(forward, left).normalized() * edgeLength;
    d = c + left;
    e = a + up;
    f = b + up;
    g = c + up;
    h = d + up;

    triangles.clear();
    addTriangle(a,b,f);
    addTriangle(f,e,a);
    addTriangle(d,a,e);
    addTriangle(e,h,d);
    addTriangle(c,d,h);
    addTriangle(h,g,c);
    addTriangle(b,c,g);
    addTriangle(g,f,b);
    addTriangle(e,f,g);
    addTriangle(g,h,e);
    addTriangle(d,c,b);
    addTriangle(b,a,d);
}

void Cube::print() const {
    std::cout << "<" << 
    "(" << a.getX() << ", " << a.getY() << ", " << a.getZ() << ")" << 
    "(" << b.getX() << ", " << b.getY() << ", " << b.getZ() << ")" << 
    "(" << c.getX() << ", " << c.getY() << ", " << c.getZ() << ")" << 
    "(" << d.getX() << ", " << d.getY() << ", " << d.getZ() << ")" << 
    "(" << e.getX() << ", " << e.getY() << ", " << e.getZ() << ")" << 
    "(" << f.getX() << ", " << f.getY() << ", " << f.getZ() << ")" << 
    "(" << g.getX() << ", " << g.getY() << ", " << g.getZ() << ")" << 
    "(" << h.getX() << ", " << h.getY() << ", " << h.getZ() << ")" << 
    ">" << std::endl;
}

// Getters
const Point& Cube::getA() const { return a; }
const Point& Cube::getB() const { return b; }
const Point& Cube::getC() const { return c; }
const Point& Cube::getD() const { return d; }
const Point& Cube::getE() const { return e; }
const Point& Cube::getF() const { return f; }
const Point& Cube::getG() const { return g; }
const Point& Cube::getH() const { return h; }
vector<Triangle>& Cube::getTriangles() { return triangles; }
const Point Cube::getCentroid() const {
    Vector forward = c - b;
    Vector left = a - b;
    Vector up = cross(forward, left);
    return b + left/2 + up/2 + forward/2;
}

// Interseção de um vetor com o triangulo
HitRecord Cube::hit(const Ray& r) const {
    if (!getBoundingBox().hit(r)) {
        return HitRecord{};
    }

    HitRecord closest{};
    HitRecord rec{};

    // Itera na lista para verificar os objetos
    for (const auto& triangle : triangles) {
        rec = triangle.hit(r);
        if (rec.t < closest.t && rec.t > 0) {
            closest = rec;
        }
    }

    if (closest.t == numeric_limits<double>::max()) {
        return HitRecord{};
    }

    HitRecord shadowRec{};
    Point start = closest.hit_point + closest.normal * t_min;
    Ray shadowRay(start, r.origin - start);

    bool inShadow = false;
    double lightDistance = (r.origin - start).magnitude(); 

    for (auto& triangle : triangles) {
        shadowRec = triangle.hit(shadowRay);
        
        // Se o raio de sombra atinge algo que é um hit válido (t > 0)
        // E esse hit está ANTES da luz (shadowRec.t < lightDistance)
        // Então, está em sombra.
        if (shadowRec.t > 0 && shadowRec.t < lightDistance) {
            inShadow = true;
            break;
        }
    }

    if (inShadow) {
        return HitRecord{};
    } else {
        if (visualTexture) {
            double u = 0, v = 0;
            double absX = std::abs(closest.normal.getX());
            double absY = std::abs(closest.normal.getY());
            double absZ = std::abs(closest.normal.getZ());

            if (absX > absY && absX > absZ) { // Face Esquerda ou Direita (+X ou -X)
                u = closest.hit_point.getZ();
                v = closest.hit_point.getY();
            } else if (absY > absZ) {         // Face de Cima ou de Baixo (+Y ou -Y)
                u = closest.hit_point.getX();
                v = closest.hit_point.getZ();
            } else {                          // Face da Frente ou de Trás (+Z ou -Z)
                u = closest.hit_point.getX();
                v = closest.hit_point.getY();
            }

            double edgeLength = (a - b).magnitude();
            u = (u / edgeLength) + 0.5;
            v = (v / edgeLength) + 0.5;
            closest.kd = visualTexture->value({u, v});
        }
        return closest;
    }
}

Hittable* Cube::clone() const {
    return new Cube(*this);
}

BoundingBox Cube::getBoundingBox() const {
    return BoundingBox(
        getA(),
        getG()
    );
}

// Todo: Todo functions
void Cube::rotateAll(double angle) {
    cout << angle << "TODO CUBE!" << endl;
}

void Cube::rotateX(double angle) {
    cout << angle << "TODO CUBE!" << endl;
}

void Cube::rotateY(double angle) {
    cout << angle << "TODO CUBE!" << endl;
}

void Cube::rotateZ(double angle) {
    cout << angle << "TODO CUBE!" << endl;
}

void Cube::transfer(Vector distances){
    cout << "TODO CUBE!" << endl;
    distances.print();
}