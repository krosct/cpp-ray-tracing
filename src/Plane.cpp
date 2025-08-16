#include "../include/Plane.h"
#include "../include/Operations.h"
#include "../include/Ray.h"
#include <iostream>

using namespace std;

Plane::Plane(string objName, Point newPlanePoint, Vector newNormal, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Plane"),
    planePoint(newPlanePoint)
    { normal = newNormal.normalized(); }

// Getters
const Point& Plane::getPlanePoint() const { return planePoint; }
const Vector& Plane::getNormal() const { return normal; }

// Setters
void Plane::setPlanePoint(Point& newPlanePoint) { planePoint = newPlanePoint; }
void Plane::setNormal(Vector& newNormal) { normal = newNormal; }

HitRecord Plane::hit (const Ray& r) const {
    // Esta função analisa os vetores N normal e vetor D direção do raio
    // P -> Origem do raio
    // Q -> Ponto no plano
    // ((P - t * D) - Q) * N = 0
    // t = ((Q-P) * n)/(n * d)

    // Produto escalar de d (direção do raio) com N (normal do plano).
    // Se dDotN == 0, então o raio é paralelo ao plano
    // Se dDotN != 0, então o raio intersecta o plano
    double dDotN = dot(r.direction, getNormal());
    
    // Vetor da origem do raio até o ponto do plano
    Vector qp = r.origin - getPlanePoint();

    // Produto escalar de qp (raio->plano) com N (normal do plano).
    // Se qpDotN == 0, então qp é paralelo ao plano e intersecta (infinitas vezes) todo o plano pois o raio está no próprio plano
    // Se qpDotN != 0, então o raio intersecta o plano
    double qpDotN = dot(qp, getNormal());
    HitRecord rec{};
    double t = 0;
    bool rayTouchPlane = false;

    if (dDotN == 0) {
        if (qpDotN == 0) {
            // Raio está contido no plano
            rayTouchPlane = true;
        }
    } else {
        // Raio intersecta o plano mas temos que verificar o t
        // Equação vetorial do plano: n . (p - pp) = 0, onde
        // n é a normal, p é o ponto que queremos verificar e
        // pp é um ponto do plano
        // Equação da reta: p(t) = pr + vt, onde
        // p(t) é o ponto que queremos descobrir, pr é um ponto da reta,
        // v é o vetor diretor da reta e t é uma escalar
        // 
        // Se substituirmos p por p(t) temos que
        // n . (pr + tv - pp) = 0
        // E fazendo várias manipulações chegamos em:
        // t = (-dot(pr, n) + dot(pp, n)) / dot(v, n)

        // Como nossa operação dot não reconhece dot de ponto com vetor, temos que 
        // subtrair do ponto que queremos com o ponto (0, 0, 0) para obter os mesmos valores
        // agora como o tipo ponto que a operação dot reconhece
        t = (-dot(r.origin-Point(0, 0, 0), getNormal()) + dot(getPlanePoint()-Point(0, 0, 0), getNormal())) / dDotN;

        if (t > 0) {
            rayTouchPlane = true;
        }
    }
    
    if (rayTouchPlane) {
        rec.t = t;
        rec.hit_point = r.origin + (rec.t * r.direction);
        rec.normal = dDotN < 0 ? getNormal() : -1 * getNormal();
        rec.ka = getka();
        rec.ks = getks();
        rec.kd = getkd();
        rec.shininess = getshininess();
        rec.kr = getkr();
        rec.kt = getkt();
        rec.obj = this;
    }
    
    return rec;
}

void Plane::rotateAll(double angle) {
    cout << angle << "TODO PLANE!" << endl;
}
void Plane::rotateX(double angle) {
    cout << angle << "TODO PLANE!" << endl;
}
void Plane::rotateY(double angle) {
    cout << angle << "TODO PLANE!" << endl;
}
void Plane::rotateZ(double angle) {
    cout << angle << "TODO PLANE!" << endl;
}
void Plane::transfer(Vector distances) {
    cout << "TODO PLANE!" << endl;
    distances.print();
}

void Plane::print() const {
    // std::cout << "Point at <" << planePoint.getX() << ", " << planePoint.getY() << ", " << planePoint.getZ() << ">" 
    // << "; Normal vector is " << normal << std::endl;
}

Hittable* Plane::clone() const {
    return new Plane(*this);
}

BoundingBox Plane::getBoundingBox() const {
    return BoundingBox();
}