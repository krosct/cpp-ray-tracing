#include "../include/Square.h"
#include "../include/Plane.h"
#include "../include/Operations.h"
#include "../include/UniversalVars.h"
#include <iostream>
#include <vector>

using namespace std;

// Construtor
Square::Square(string objName, Point a, Point c, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture, Texture* heightTexture) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Square", visualTexture, heightTexture),
    a(a),
    c(c)
{
    b = Point(c.getX(), a.getY(), c.getZ());
    d = Point(a.getX(), c.getY(), a.getZ());
    recalculateNormal();
}

// Print do quadrado
void Square::print() const {
    std::cout << "<" << 
    "(" << a.getX() << ", " << a.getY() << ", " << a.getZ() << ")" << 
    "(" << b.getX() << ", " << b.getY() << ", " << b.getZ() << ")" << 
    "(" << c.getX() << ", " << c.getY() << ", " << c.getZ() << ")" <<
    "(" << d.getX() << ", " << d.getY() << ", " << d.getZ() << ")" <<
    ">" << std::endl;
}

// Getters
const Point& Square::getA() const { return a; }
const Point& Square::getB() const { return b; }
const Point& Square::getC() const { return c; }
const Point& Square::getD() const { return d; }
const Vector& Square::getNormal() const { return normal; }
const Point Square::getCentroid() const {
    Vector diag = c - a;
    return a + (diag/2);
}

// Setters
void Square::setA(Point newA) { a = newA; }
void Square::setB(Point newB) { b = newB; }
void Square::setC(Point newC) { c = newC; }
void Square::setD(Point newD) { d = newD; }
void Square::setNormal(Vector newNormal) { normal = newNormal; }
void Square::recalculateNormal() {
    setNormal(cross(b - a, d - a).normalized());
}

// Interseção otimizada
HitRecord Square::hit(const Ray& r) const {
    // Plano da face do quadrado
    Plane p = Plane("tempPlane", getA(), getNormal(), getka(), getkd(), getks(), getshininess(), getkr(), getkt());
    // rec guarda as informações se r intersecta o plano p
    HitRecord rec = p.hit(r);

    // Se r não intersecta o plano então também não intersecta o quadrado
    // Se r intersecta o plano então temos que verificar se intersecta o quadrado
    // Se alfa, beta e gama forem diferentes de 0 então r intersecta o quadrado
    if (rec.t == numeric_limits<double>::max() || rec.t < t_min) { return HitRecord{}; }

    pair<double, double> vu = has(rec.hit_point);
    double u = vu.first;
    double v = vu.second;
    if (u != -1) {
        rec.obj = this;
        Vector edge1 = b - a;
        // As coordenadas baricêntricas (u, v) são usadas como coordenadas de textura.
        // O mapeamento padrão para um quadrado é: A=(0,0), B=(1,0), C=(0,1)
        // Nossas coordenadas u e v correspondem a B e C, respectivamente.
        // if (heightTexture) {
        //     ReliefResult relief = reliefMapping(r, rec.hit_point, getNormal(), heightTexture, u, v);
        //     rec.hit_point = relief.displacedHitPoint;
        //     rec.normal = relief.perturbedNormal;
        // } else {
        //     // rec.hit_point já definido pela intersecção com o plano
        //     rec.normal = dot(r.direction, rec.normal) < 0 ? rec.normal : -1 * rec.normal;
        // }

        if (visualTexture) {
            rec.kd = visualTexture->value({u, v});
        } else {
            rec.kd = getkd();
        }

        rec.normal = dot(r.direction, getNormal()) < 0 ? getNormal() : -1 * getNormal();
        rec.ka = getka();
        rec.ks = getks();
        rec.shininess = getshininess();
        rec.kr = getkr();
        rec.kt = getkt();
        return rec;
    } else {
        return HitRecord{};
    }
}

Hittable* Square::clone() const {
    return new Square(*this);
}

pair<double, double> Square::has(Point p) const {
    Vector ad = d - a;
    Vector ab = b - a;
    Vector ap = p - a;

    double dot_ap_ab = dot(ap, ab);
    double dot_ab_ab = dot(ab, ab);
    double dot_ap_ad = dot(ap, ad);
    double dot_ad_ad = dot(ad, ad);

    double u;
    double v;
    try
    {
        u = dot_ap_ab / dot_ab_ab;
        v = dot_ap_ad / dot_ad_ad;
    }
    catch(const std::exception& e)
    {
        return {-1,-1};
    }

    if (u < t_min || v < t_min || u > 1 || v > 1) { return {-1,-1}; }

    return {u, v};
}

// Rotações e Translação
void Square::rotateAll(double angle) {
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotation(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
    setD(getD() * result);
}

void Square::rotateX(double angle) {
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationX(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
    setD(getD() * result);

    recalculateNormal();
}

void Square::rotateY(double angle) {
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationY(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
    setD(getD() * result);
}

void Square::rotateZ(double angle) {
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationZ(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
    setD(getD() * result);
}

void Square::transfer(Vector distances){
    Matrix m(4);
    m.buildTranslation(distances);
    
    setA(m*getA());
    setB(m*getB());
    setC(m*getC());
    setD(m*getD());
}

BoundingBox Square::getBoundingBox() const {
    return BoundingBox(
        getA(),
        getC()
    );
}