#include "../include/Hittable.h"
#include "../include/Texture.h"
#include "../include/Matrix.h"
#include "../include/Ray.h"
#include "../include/Operations.h"
#include "../include/Sphere.h"
#include "../include/Square.h"
#include "../include/Plane.h"
#include "../include/Triangle.h"
#include "../include/Rectangle.h"
#include "../include/Cube.h"
#include "../include/Cylinder.h"
#include "../include/Camera.h"
#include "../include/Light.h"
#include "../include/World.h"
#include "../include/Colours.h"
#include "../include/UniversalVars.h"
// #include "src/PpmP3.cpp"
#include <random>
#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <fstream>

using namespace std;
const double t_min = 0.00001;
const double maxDouble = numeric_limits<double>::max();
const double minDouble = numeric_limits<double>::min();

Vector phongColour(World* w, const unordered_map<string, ExtensiveLight*>& pontualLights, const EnvLight& ambientLight, const Point& cameraLocation, const Ray& r, unsigned& recursionDepth, bool noShadow);

Vector environmentComponent(const Vector& ka, const Vector& colour) {
    return (ka * colour);
}

Vector diffuseComponent(const Vector& kd, const Vector& N, const Vector& L, const PontualLight* light) {
    // Componente Difusa: I_Li * k_d * (N . L_i)
    double NdotL = max(0.0, dot(N, L)); // Qnt menor angulo, result mais prox de 1
    Vector diffuse_contrib = kd * NdotL;
    diffuse_contrib = light->getColour() * diffuse_contrib;
    return diffuse_contrib;
}

Vector specularComponent(const Vector& ks, const Vector& V, const Vector& N, const Vector& L, const PontualLight* light, const double& shininess) {
    // Componente Especular: I_Li * k_s * (R_i . V)^n
    Vector R = (2 * dot(N, L) * N - L).normalized();
    double RdotV = max(0.0, dot(R, V));
    double shininess_term = pow(RdotV, shininess);
    Vector specular_contrib = ks * shininess_term;
    specular_contrib = light->getColour() * specular_contrib;
    return specular_contrib;
}

Vector reflectiveComponent(World* w, const unordered_map<string, ExtensiveLight*>& pontualLights, const EnvLight& ambientLight, unsigned& recursionDepth, const Vector& V, const Vector& N, const Point& hitPoint, const double& kr, const double& t_min, bool noShadow) {
    // Reflexão:
    // Fórmula da Reflexão da Monitoria:
    Vector reflectionDirection = (2 * N * (dot(N, V)) - V);
    Vector reflection_contrib = phongColour(w, pontualLights, ambientLight, hitPoint, Ray(hitPoint + (N * t_min), reflectionDirection), ++recursionDepth, noShadow);
    return (reflection_contrib * kr);
}

Vector refractiveComponent(World* w, const unordered_map<string, ExtensiveLight*>& pontualLights, const EnvLight& ambientLight, unsigned& recursionDepth, const Vector& N, const Vector& L, const Point& hitPoint, const double& originRefraction, const double& destinyRefraction, const double& t_min, bool noShadow) {
    // Refração:
    // I é a direção do raio incidente. Como V aponta do hit_point para a câmera,
    // o raio incidente vem da câmera para o hit_point, então I = -V.
    // I = Exterior -> OBJ
    // N = OBJ -> Exterior

    double Nzao = originRefraction / destinyRefraction;
    double cosTeta = dot(N, L);
    double cosTetaT = sqrt(1 - ( (Nzao * Nzao) * (1 - (cosTeta * cosTeta)) ));
    if (cosTeta < 0) { cosTetaT = -cosTetaT; }
    Vector T = ( Nzao * (-1 * L) ) + ( (Nzao * cosTeta) - (cosTetaT) ) * N;
    Vector antiBug = N * (cosTeta > 0 ? -1 : 1);
    Vector refraction_contrib = phongColour(w, pontualLights, ambientLight, hitPoint, Ray(hitPoint + antiBug * t_min, T), ++recursionDepth, noShadow);
    return refraction_contrib;
}

bool inShadow(const HitRecord& rec, const Vector& rawL, const Vector& L, const Vector& N, World* w, const double& t_min) {
    HitRecord shadowRec{};
    Ray shadowRay(rec.hit_point + N * t_min, L);

    double lightDistance = rawL.magnitude();

    // Verifica a interseção com a Octree
    const Octree* octree = w->getOctree();
    if (octree) {
        HitRecord shadowRec = octree->hit(shadowRay);
        // Se o raio de sombra atingiu algo (t > 0) e esse algo está ANTES da luz (t < lightDistance)
        // então o ponto está em sombra.
        if (shadowRec.t > t_min && shadowRec.t < lightDistance) {
            return true; // Objeto da Octree bloqueou a luz
        }
    }

    // Verifica objetos que não estão na Octree
    for (auto const& [key, obj] : w->getObjList()) {
        // A condição para identificar um objeto que não está na Octree
        if (obj->getBoundingBox().min.getX() == numeric_limits<double>::max()) {
            // Ignora o próprio objeto de onde o raio parte
            if (obj->getName() == rec.obj->getName()) {
                continue;
            }

            HitRecord unboundedRec = obj->hit(shadowRay);
            if (unboundedRec.t > t_min && unboundedRec.t < lightDistance) {
                return true; // Objeto infinito bloqueou a luz
            }
        }
    }

    // Se nada bloqueou o raio, não está na sombra
    return false;
}

Vector phongColour(World* w, const unordered_map<string, ExtensiveLight*>& pontualLights, const EnvLight& ambientLight, const Point& cameraLocation, const Ray& r, unsigned& recursionDepth, bool noShadow = false) {
    if (recursionDepth >= 4) {
        return Vector(0,0,0); 
    }

    // t_min é crucial para raios de sombra e evitar z-fighting.
    const double t_min = 0.00001; 

    HitRecord closest = HitRecord{};

    const Octree* octree = w->getOctree();
    if (octree) {
        closest = octree->hit(r);
    } else {
        cerr << "ERROR: world does not have a Octree!" << endl;
        return {red(170), green(170), blue(170)};
    }
    // Teste separado dos objetos que não estão no Octree (os filtrados)
    std::vector<Hittable*> objs;
    // Filtra objetos problemáticos
    for (auto const& [key, obj] : w->getObjList()) {
        if (obj->getBoundingBox().min.getX() == numeric_limits<double>::max()) {
            HitRecord plane_rec = obj->hit(r);
            if (plane_rec.t > t_min && plane_rec.t < closest.t) {
                closest = plane_rec;
            }
        }
    }
    // Se closest.t ainda for o valor máximo, significa que nenhum objeto foi atingido (ou todos tiveram t <= 0).
    // Ou seja, a cor de fundo deve ser retornada.
    if (closest.t == numeric_limits<double>::max()) {
        return {red(170), green(170), blue(170)}; // Retorna a cor de fundo
    }

    if (closest.obj->getName().find("light") != string::npos) {
        return YELLOW;
    }

    Vector finalColor = Vector(0, 0, 0);

    // 1. Componente Ambiente (ka * Ia)
    if (closest.ka != Vector{0,0,0}) { finalColor += environmentComponent(closest.ka, ambientLight.getColour()); }

    // 2. Componente Difusa e Especular para cada luz pontual
    Vector N = closest.normal.normalized();  // Vetor normal da superfície. É um vetor unitário que aponta para fora da superfície no ponto que está sendo iluminado.
    Vector V = (cameraLocation - closest.hit_point).normalized(); // Vetor de visão. É o vetor unitário que aponta da superfície para a câmera (o observador).
    for (const auto& light : pontualLights) {
        Vector semiFinalColor = {0, 0, 0};
        for (const auto& pontualLight : light.second->getLights()) {
            Vector lightColor = pontualLight->getColour();
            Point lightLocation = pontualLight->getLocation();

            Vector rawL = (lightLocation - closest.hit_point);
            Vector L = rawL.normalized(); // Vetor da luz. É um vetor unitário que aponta da superfície para a fonte de luz.

            bool objInShadow = false;
            if (!noShadow) {
                objInShadow = inShadow(closest, rawL, L, N, w, t_min);
            }

            if (!objInShadow) {
                if (closest.kd != Vector{0,0,0}) { semiFinalColor += diffuseComponent(closest.kd, N, L, pontualLight); }

                if (closest.ks != Vector{0,0,0}) { semiFinalColor += specularComponent(closest.ks, V, N, L, pontualLight, closest.shininess); }

                if (closest.kr != 0) { semiFinalColor += reflectiveComponent(w, pontualLights, ambientLight, recursionDepth, V, N, closest.hit_point, closest.kr, t_min, noShadow); }

                if (closest.kt != 0) { semiFinalColor += refractiveComponent(w, pontualLights, ambientLight, recursionDepth, N, L, closest.hit_point, 1, closest.kt, t_min, noShadow); }
            }
        }
        semiFinalColor = semiFinalColor / light.second->getNumLights();
        finalColor += semiFinalColor;
    }
    
    // Garantir que os componentes RGB não excedam [0,1]
    finalColor.setX(min(1.0, finalColor.getX()));
    finalColor.setY(min(1.0, finalColor.getY()));
    finalColor.setZ(min(1.0, finalColor.getZ()));

    return finalColor;
}

string paintScreen(const int& i, const int& j, const Camera& c, World* w, const unordered_map<string, ExtensiveLight*>& pontualLights, const EnvLight& ambientLight, bool noShadow) {
    // Normaliza as coordenadas do pixel para (u,v) entre 0.0 e 1.0
    double u = double(i) / double(c.getHRes());
    double v = double(j) / double(c.getVRes());
    
    Point target = c.getScreen().lower_left_corner + (u * c.getScreen().horizontal) + (v * c.getScreen().vertical);
    Ray r = Ray(c.getLocation(), target - c.getLocation());
    
    // Calcula a cor para o raio, passando a lista de objetos
    unsigned recursionDepth = 0;
    Vector colour = phongColour(w, pontualLights, ambientLight, c.getLocation(), r, recursionDepth, noShadow);

    // Converte a cor para o formato de 0 a 255 e imprime
    unsigned ir = int(255 * colour.getX());
    unsigned ig = int(255 * colour.getY());
    unsigned ib = int(255 * colour.getZ());

    return (to_string(ir) + " " + to_string(ig) + " " + to_string(ib));
}

// Função para exibir a barra de progresso
void exibir_barra_de_loading(unsigned porcentagem) {
    // Define a largura da barra no terminal
    unsigned largura_barra = 20;
    
    // Calcula quantas posições da barra devem ser preenchidas
    unsigned pos = (largura_barra * porcentagem)/100;

    // Retorna o cursor para o início da linha
    std::cout << "\r["; 

    // Desenha a parte preenchida da barra
    for (unsigned i = 0; i < largura_barra; ++i) {
        if (i < pos) {
            std::cout << "#";
        } else {
            std::cout << "-";
        }
    }
    
    // Exibe a porcentagem e um espaço para garantir que a linha antiga seja sobrescrita
    std::cout << "] " << porcentagem << "% ";
    std::cout.flush();
}

int main() {
    string fileName = "novafoto";
    ofstream exitRGB("img/" + fileName + ".ppm");

    World* w = new World();

    bool noShadow = false;
    bool useParallel = false;
    unsigned cenario = 0;
    
    if (cenario == 0) {
        Texture* t = new Texture("img/originalChess.ppm");
        w->add(Camera("cam1", {0, 2, 5}, {0, 0, -1}, {0, 1, 0}));
        w->add(EnvLight("env1", {0.3, 0.3, 0.3}));
        w->add(ExtensiveLight("luz_teto00", {0.6,0.6,0.6}, {-5,10,2}, 1));
        DARK_GRAY.print();
        WHITE.print();
        w->add(Square("backWall", {-25,-3,-20}, {25,15,-20}, DARK_GRAY, DARK_GRAY, WHITE, 5, 0.03, 0));
        w->add(Square("floor", {-25,-23,-10}, {25,17,-10}, DARK_GRAY, DARK_GRAY, WHITE, 1000, 0.05, 0, t));
        w->getObjList().find("floor")->second->rotateX(rad(90));
        w->add(Cylinder("cylinder1", {-7,-3,-7}, 1, 2, RED, RED, WHITE, 1000, 0, 0));
        w->add(Cylinder("cylinder2", {0,-3,-5}, 2, 5.3, GREEN, GREEN, WHITE, 1000, 0, 0));
        w->add(Cylinder("cylinder3", {2.25,-0.5,-1}, 0.3, 3, BLUE, BLUE, WHITE, 1000, 0, 0));
        w->add(Cube("cube1",{-7,-3,-1}, {-5,-3,-1}, YELLOW, YELLOW, WHITE, 1000, 0, 0));
        w->add(Cube("cube2",{1,-3,0}, {3.5,-3,0}, CYAN, CYAN, WHITE, 1000, 0, 0));
        w->add(Cube("cube3",{10,-3,-9}, {13,-3,-9}, MAGENTA, MAGENTA, WHITE, 1000, 0, 0));
        w->add(Sphere("sphere1", {-1,-2,0}, 1, ORANGE, ORANGE, WHITE, 100, 0, 0));
        w->add(Sphere("sphere2", {-10,1,-15}, 4, PURPLE, PURPLE, WHITE, 100, 0, 0));
        w->add(Sphere("sphere3", {11.5,2,-10.5}, 2, BROWN, BROWN, WHITE, 100, 0, 0));
    } else {
        // default case
    }
    
    w->print();
    w->buildOctree();
    Camera cam1 = *(w->getCameraList().find("cam1")->second);
    int hres = cam1.getHRes();
    int vres = cam1.getVRes();
    long long progTotal = hres * vres;
    long long moduleWriteLoadingBar = progTotal/10;
    long long prog = 1;
    vector<string> pixelBuffer(progTotal);

    if (useParallel) {
        cout << "Iniciando renderização!" << endl;
        #pragma omp parallel for schedule(dynamic)
        for (int j = vres - 1; j >= 0; j--) { // de cima para baixo
            for (int i = 0; i < hres; i++) { // da esquerda para direita
                string pixelColor;
                pixelColor = paintScreen(
                    i,
                    j,
                    cam1,
                    w,
                    w->getPontualLightList(),
                    w->getEnvLight(),
                    noShadow
                );
                // pixelBuffer[j * hres + i] = pixelColor;
                pixelBuffer[(vres - 1 - j) * hres + i] = pixelColor;
    
                #pragma omp atomic
                prog++;
    
                if (prog % moduleWriteLoadingBar == 0) {
                    #pragma omp critical
                    exibir_barra_de_loading(prog*100/progTotal);
                }
            }
        }
        
        cout << endl;
        exitRGB << "P3\n" << hres << " " << vres << "\n255\n";
        for (int i = 0; i < progTotal; ++i) {
            exitRGB << pixelBuffer[i] << endl;
        }
    } else {

        for (int j = vres - 1; j >= 0; j--) { // de cima para baixo
            for (int i = 0; i < hres; i++) { // da esquerda para direita
                prog ++;
                if (prog % moduleWriteLoadingBar == 0) {
                    exibir_barra_de_loading(prog*100/progTotal);
                }
                string pixelColor;
                pixelColor = paintScreen(
                    i,
                    j,
                    cam1,
                    w,
                    w->getPontualLightList(),
                    w->getEnvLight(),
                    noShadow
                );
                pixelBuffer[(vres - 1 - j) * hres + i] = pixelColor;
            }
        }
        cout << endl;
        exitRGB << "P3\n" << hres << " " << vres << "\n255\n";
        for (int i = 0; i < progTotal; ++i) {
            exitRGB << pixelBuffer[i] << endl;
        }
    }
    
    exitRGB.close();
    cout << "Arquivo '" << fileName << ".ppm' criado na pasta 'img' com sucesso!" << endl;
    return 0;
}
