#include "../include/Rectangle.h"
#include "../include/Operations.h"
#include "../include/Matrix.h"
#include <iostream>

using namespace std;

Rectangle::Rectangle(string objName, Point p0_in, double width, double length, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Rectangle"),
    p0(p0_in)
{
    // Largura vai no X
    edgeU = Vector(width, 0, 0);

    // Comprimento vai no Y
    edgeV = Vector(0, length, 0);

    // Normal apontando para +Z
    normal = cross(edgeU, edgeV).normalized();
}

// Getters
const Point& Rectangle::getP0() const { return p0; }

const Vector& Rectangle::getWidth() const { return edgeU; }

const Vector& Rectangle::getLength() const { return edgeV; }

const Vector& Rectangle::getNormal() const { return normal; }

// Setters
void Rectangle::setP0(Point newP0) { p0 = newP0; }

void Rectangle::setWidth(double newWidth) { edgeU = Vector(newWidth, 0, 0); }

void Rectangle::setLength(double newLength) { edgeV = Vector(0, newLength, 0); }

void Rectangle::setNormal(Vector newNormal) { normal = newNormal; }

HitRecord Rectangle::hit(const Ray& r) const {
    HitRecord rec{};

    double denom = dot(r.direction, normal);
    if (denom == 0)
        return HitRecord{}; // Raio paralelo

    double t = dot(p0 - r.origin, normal) / denom;
    if (t < 0)
        return HitRecord{}; // Atrás do olho

    Point p = r.origin + t * r.direction;
    Vector d = p - p0;

    double u = dot(d, edgeU.normalized());
    double v = dot(d, edgeV.normalized());

    if (u < 0 || u > edgeU.magnitude() || v < 0 || v > edgeV.magnitude())
        return HitRecord{}; // Fora do retângulo

    rec.t = t;
    rec.hit_point = p;
    rec.ka = getka();
    rec.ks = getks();
    rec.kd = getkd();
    rec.kr = getkr();
    rec.kt = getkt();
    rec.shininess = getshininess();
    rec.obj = this;

    // Normal sempre apontando para fora
    rec.normal = (denom < 0) ? normal : -1 * normal;

    return rec;
}

void Rectangle::rotateX(double angle) {
    Point pivot = p0 + 0.5 * edgeU + 0.5 * edgeV;
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationX(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    Point p1 = p0 + edgeU;
    Point p2 = p0 + edgeV;
    
    p0 = p0 * result;
    p1 = p1 * result;
    p2 = p2 * result;
    edgeU = p1 - p0;
    edgeV = p2 - p0;

    normal = cross(edgeU, edgeV).normalized();
}

void Rectangle::rotateY(double angle) {
    Point pivot = p0 + 0.5 * edgeU + 0.5 * edgeV;
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationY(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    Point p1 = p0 + edgeU;
    Point p2 = p0 + edgeV;
    
    p0 = p0 * result;
    p1 = p1 * result;
    p2 = p2 * result;
    edgeU = p1 - p0;
    edgeV = p2 - p0;

    normal = cross(edgeU, edgeV).normalized();
}

void Rectangle::rotateZ(double angle) {
    Point pivot = p0 + 0.5 * edgeU + 0.5 * edgeV;
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationZ(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    Point p1 = p0 + edgeU;
    Point p2 = p0 + edgeV;
    
    p0 = p0 * result;
    p1 = p1 * result;
    p2 = p2 * result;
    edgeU = p1 - p0;
    edgeV = p2 - p0;

    normal = cross(edgeU, edgeV).normalized();
}

void Rectangle::rotateAll(double angle) {
    cout << angle << "TODO RECTANGLE!" << endl;
}

void Rectangle::transfer(Vector d) {
    p0 = p0 + d;
}

void Rectangle::print() const {
    std::cout << "Rectangle <P0: (" << p0.getX() << ", " << p0.getY() << ", " << p0.getZ()
              << "), Width: " << edgeU.magnitude()
              << ", Length: " << edgeV.magnitude()
              << ">" << std::endl;
}

Hittable* Rectangle::clone() const {
    return new Rectangle(*this);
}

BoundingBox Rectangle::getBoundingBox() const {
    return BoundingBox(
        getP0(),
        getP0()+getWidth()+getLength()
    );
}