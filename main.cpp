#include "src/Hittable.h"
#include "src/Matrix.h"
#include "src/Ray.h"
#include "src/Operations.h"
#include "src/Sphere.h"
#include "src/Plane.h"
#include "src/Triangle.h"
#include "src/Rectangle.h"
#include "src/Cube.h"
#include "src/Camera.h"
#include "src/Light.h"
#include "src/Colormap.cpp"
#include "src/ObjReader.cpp"
#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include "src/Colours.h"

using namespace std;
Vector phongColour(const vector<Hittable*>& objList, const vector<Light::PontualLight*>& pontualLights, const Light::EnvLight& ambientLight, const Point& cameraLocation, const Ray& r, int recursionDepth);

// Função que define a cor de fundo
Vector simpleColour(const vector<Hittable*>& l,  const Ray& r) {

    HitRecord closest = HitRecord{
        numeric_limits<double>::max(),
        {0,0,0}, {0,0,0}, {0,0,0},
        {red(238), green(238), blue(228)}, // Cor de fundo padrão
        {0,0,0}, 0.0, 0.0, 0.0
    };
    HitRecord rec;

    // Itera na lista para verificar os objetos
    for (const auto& obj : l) {
        rec = obj->hit(r);
        if (rec.t != 0) {
            if (rec.t < closest.t) {
                closest = rec;
            }
        }
    }

    return closest.kd;
}

Vector environmentComponent(Vector ka, Vector colour) {
    return (ka * colour);
}

Vector diffuseComponent(Vector kd, Vector N, Vector L, const Light::PontualLight& light) {
    // Componente Difusa: I_Li * k_d * (N . L_i)
    double NdotL = max(0.0, dot(N, L)); // Qnt menor angulo, result mais prox de 1
    Vector diffuse_contrib = kd * NdotL;
    diffuse_contrib = light.getColour() * diffuse_contrib;
    return diffuse_contrib;
}

Vector specularComponent(Vector ks, Vector V, Vector N, Vector L, const Light::PontualLight& light, double shininess) {
    // Componente Especular: I_Li * k_s * (R_i . V)^n
    Vector R = (2 * dot(N, L) * N - L).normalized();
    double RdotV = max(0.0, dot(R, V));
    double shininess_term = pow(RdotV, shininess);
    Vector specular_contrib = ks * shininess_term;
    specular_contrib = light.getColour() * specular_contrib;
    return specular_contrib;
}

Vector reflectiveComponent(const vector<Hittable*>& objList, const vector<Light::PontualLight*>& pontualLights, const Light::EnvLight& ambientLight, int& recursionDepth, Vector V, Vector N, Point hitPoint, double kr, double t_min) {
    // Reflexão:
    // Fórmula da Reflexão da Monitoria:
    Vector reflectionDirection = (2 * N * (dot(N, V)) - V);
    Vector reflection_contrib = phongColour(objList, pontualLights, ambientLight, hitPoint, Ray(hitPoint + (N * t_min), reflectionDirection), ++recursionDepth);
    return (reflection_contrib * kr);
}

Vector refractiveComponent(const vector<Hittable*>& objList, const vector<Light::PontualLight*>& pontualLights, const Light::EnvLight& ambientLight, int& recursionDepth, Vector N, Vector L, Point hitPoint, double originRefraction, double destinyRefraction, double t_min) {
    // Refração:
    // I é a direção do raio incidente. Como V aponta do hit_point para a câmera,
    // o raio incidente vem da câmera para o hit_point, então I = -V.
    // I = Exterior -> OBJ
    // N = OBJ -> Exterior

    // double Nzao = destinyRefraction/originRefraction;
    double Nzao = originRefraction / destinyRefraction;
    double cosTeta = dot(N, L);
    double cosTetaT = sqrt(1 - ( (Nzao * Nzao) * (1 - (cosTeta * cosTeta)) ));
    if (cosTeta < 0) { cosTetaT = -cosTetaT; }
    Vector T = ( Nzao * (-1 * L) ) + ( (Nzao * cosTeta) - (cosTetaT) ) * N;
    Vector antiBug = N * (cosTeta > 0 ? 1 : -1);
    Vector refraction_contrib = phongColour(objList, pontualLights, ambientLight, hitPoint, Ray(hitPoint + antiBug * t_min, T), ++recursionDepth);
    return refraction_contrib;
}

bool inShadow(Point hitPoint, Vector rawL, Vector N, const vector<Hittable*>& objList, double t_min) {
    // 3. Gerenciamento de Sombras
    // Adiciona um pequeno offset à origem do raio de sombra para evitar auto-sombreamento
    // É CRUCIAL que o raio de sombra não comece exatamente no ponto de interseção para evitar "acne" de sombra.
    HitRecord shadowRec;
    Ray shadowRay(hitPoint + N * t_min, rawL);

    bool inShadow = false;
    double lightDistance = rawL.magnitude();

    for (const auto& shadowObj : objList) {
        shadowRec = shadowObj->hit(shadowRay);
        
        // Se o raio de sombra atinge algo que é um hit válido (t > 0)
        // E esse hit está ANTES da luz (shadowRec.t < lightDistance)
        // Então, está em sombra.
        if (shadowRec.t > 0 && shadowRec.t < lightDistance) {
            inShadow = true;
            break;
        }
    }

    return inShadow;
}

Vector phongColour(const vector<Hittable*>& objList, const vector<Light::PontualLight*>& pontualLights, const Light::EnvLight& ambientLight, const Point& cameraLocation, const Ray& r, int recursionDepth) {
    if (recursionDepth >= 4) {
        return Vector(0,0,0); 
    }

    // t_min é crucial para raios de sombra e evitar z-fighting.
    const double t_min = 0.00001; 

    HitRecord closest = HitRecord{
        numeric_limits<double>::max(), // distância t = max(); t: a distância ao longo do ray onde ocorreu a colisão
        {0,0,0}, // ponto de colisão
        {0,0,0}, // Vetor normal no9 ponto de colisão
        {0,0,0}, // ka: componente ambiente
        {red(238), green(238), blue(228)}, // Cor de fundo padrão; kd: difusa
        {0,0,0}, //ks especular
        0.0, // n rugosidade 
        0.0, // ks
        0.0 // kt
    };
    HitRecord current_rec;

    // Itera na lista para verificar os objetos
    for (const auto& obj : objList) {
        current_rec = obj->hit(r); 
        
        if (current_rec.t < closest.t && current_rec.t > 0) {
            closest = current_rec;
        }
    }

    // Se closest.t ainda for o valor máximo, significa que nenhum objeto foi atingido (ou todos tiveram t <= 0).
    // Ou seja, a cor de fundo deve ser retornada.
    if (closest.t == numeric_limits<double>::max()) {
        return closest.kd; // Retorna a cor de fundo
    }

    Vector finalColor = Vector(0, 0, 0);

    // 1. Componente Ambiente (ka * Ia)
    if (closest.ka != Vector{0,0,0}) { finalColor += environmentComponent(closest.ka, ambientLight.getColour()); }

    // 2. Componente Difusa e Especular para cada luz pontual
    for (const auto& light : pontualLights) {
        Vector lightColor = light->getColour();
        Point lightLocation = light->getLocation();

        Vector rawL = (lightLocation - closest.hit_point);
        Vector L = rawL.normalized(); // Vetor da luz. É um vetor unitário que aponta da superfície para a fonte de luz.
        Vector N = closest.normal;  // Vetor normal da superfície. É um vetor unitário que aponta para fora da superfície no ponto que está sendo iluminado.
        Vector V = (cameraLocation - closest.hit_point).normalized(); // Vetor de visão. É o vetor unitário que aponta da superfície para a câmera (o observador).

        bool objInShadow = inShadow(closest.hit_point, rawL, N, objList, t_min);

        if (!objInShadow) {
            if (closest.kd != Vector{0,0,0}) { finalColor += diffuseComponent(closest.kd, N, L, *light); }

            if (closest.ks != Vector{0,0,0}) { finalColor += specularComponent(closest.ks, V, N, L, *light, closest.shininess); }

            if (closest.kr != 0) { finalColor += reflectiveComponent(objList, pontualLights, ambientLight, recursionDepth, V, N, closest.hit_point, closest.kr, t_min); }

            if (closest.kt != 0) { finalColor += refractiveComponent(objList, pontualLights, ambientLight, recursionDepth, N, L, closest.hit_point, 1, closest.kt, t_min); }
        }
    }
    
    // Garantir que os componentes RGB não excedam [0,1]
    finalColor.setX(min(1.0, finalColor.getX()));
    finalColor.setY(min(1.0, finalColor.getY()));
    finalColor.setZ(min(1.0, finalColor.getZ()));

    return finalColor;
}

string paintScreen(int i, int j, Camera c, const vector<Hittable*>& objList, const vector<Light::PontualLight*>& pontualLights, const Light::EnvLight& ambientLight) {
    // Normaliza as coordenadas do pixel para (u,v) entre 0.0 e 1.0
    double u = double(i) / double(c.getHRes());
    double v = double(j) / double(c.getVRes());
    
    Point target = c.getScreen().lower_left_corner + (u * c.getScreen().horizontal) + (v * c.getScreen().vertical);
    Ray r = Ray(c.getLocation(), target - c.getLocation());
    
    // Calcula a cor para o raio, passando a lista de objetos
    Vector col = phongColour(objList, pontualLights, ambientLight, c.getLocation(), r, 0);

    // Converte a cor para o formato de 0 a 255 e imprime
    int ir = int(255 * col.getX());
    int ig = int(255 * col.getY());
    int ib = int(255 * col.getZ());

    return (to_string(ir) + " " + to_string(ig) + " " + to_string(ib));
}

// Função para exibir a barra de progresso
void exibir_barra_de_loading(int progresso, int total) {
    // Calcula a porcentagem
    float porcentagem = (float)progresso / total;
    
    // Define a largura da barra no terminal
    int largura_barra = 50;
    
    // Calcula quantas posições da barra devem ser preenchidas
    int pos = largura_barra * porcentagem;

    // Retorna o cursor para o início da linha
    std::cout << "\r["; 

    // Desenha a parte preenchida da barra
    for (int i = 0; i < largura_barra; ++i) {
        if (i < pos) {
            std::cout << "#";
        } else {
            std::cout << "-";
        }
    }
    
    // Exibe a porcentagem e um espaço para garantir que a linha antiga seja sobrescrita
    std::cout << "] " << int(porcentagem * 100.0) << "% ";

    // Força a saída do buffer para o terminal imediatamente
    std::cout.flush();
}

int main() {
    string fileName = "italoDemonstration";
    // Camera c = Camera({-0.5, 1.5, 1}, {0, 0, -1}, {0, 1, 0});
    Camera c = Camera({0, 0, 1}, {0, 0, -1}, {0, 1, 0});
    // X -> Para direita ou para esquerda
    // y -> Para cima ou para baixo
    // Z -> Para dentro ou para fora da tela
    ofstream exitRGB("img/" + fileName + ".ppm");
    vector<Hittable*> objList;
    vector<Light::PontualLight*> pontualLights;
    
    // string objFile = "inputs/Tree1.obj";
    // ObjReader objReader(objFile);
    // vector<Face> objectFaces = objReader.getFaces();
    // vector<vector<Point>> facePointsList = objReader.getFacePoints();

    // ----------------------------------------------------
    // Inicio da lista de luzes

    Light::EnvLight ambientLight = Light::EnvLight(Vector(0.3, 0.3, 0.3));
    Light::PontualLight l1 = Light::PontualLight({0, 1, 1}, {0.6,0.6,0.6});
    // Light::PontualLight l2 = Light::PontualLight({0.75, 1, -1.25}, {0.6,0.6,0.6});
    pontualLights.push_back(&l1);
    // pontualLights.push_back(&l2);

    // Fim da lista de luzes
    // ----------------------------------------------------

    // ----------------------------------------------------
    // Inicio da lista de objetos
    // Primeira cor: cor quando na sombra
    // Segunda cor: cor quando iluminado
    // Terceira cor: cor do brilho
    // kr: O quanto vai refletir os objetos em volta
    // kt: O quanto é transparente

    // Cubo
    // Cube c1 = Cube({-5,-3,3}, {5,-3,3}, BLACK, RED, WHITE, 10, 0, 0, false);
    // Cube c1 = Cube({0.5,0,-1}, {1,0,-1}, BLUE, RED, WHITE, 10, 0.3, 0, false);
    // Cube c2 = Cube({-2,-0.5,-1}, {-0.5,-0.5,-1}, BLUE, BLUE, WHITE, 10, 0.3, 0, true);

    // Plano
    Plane p1 = Plane({0,0,-30},{0,0,1},CYAN,RED,BLUE,0,0,0);

    // Triangulo
    // Triangle t1 = Triangle({2,0,0},{0,2,0},{0,0,0},CYAN,RED,BLUE,0,0,0);

    // Esferas
    // Sphere s1 = Sphere({2,0.2,-4}, 1, BLACK, BLACK, BLACK, 0, 0, 0);
    Sphere s1 = Sphere({1,0.5,-2}, 0.3, GREEN, GREEN, WHITE, 50, 0.3, 0);
    Sphere s2 = Sphere({1,0.5,-2}, 1, BLACK, BLACK, BLACK, 0, 0, 0.3);
    // Sphere s2 = Sphere({1,0.5,-2}, 1, RED, RED, LIGHT_GRAY, 1000, 0, 0.3);
    Sphere s3 = Sphere({-3,-1,-4.7}, 0.5, BLUE, DARK_GRAY, WHITE, 50, 0, 0);
    Sphere s4 = Sphere({-2,1,-2}, 1, YELLOW/6, CYAN/3, GRAY, 30, 0.25, 0);
    // Sphere s5 = Sphere({1.3,-0.3,0}, 0.15, YELLOW, CYAN/3, GRAY, 30, 0.25, 0);

    // Sphere sl1 = Sphere(l1.getLocation()+Vector{0,0.05,0}, 0.02, YELLOW, YELLOW, YELLOW, 500, 0, 0);
    // Sphere sl2 = Sphere(l2.getLocation()+Vector{0,0.05,0}, 0.02, YELLOW, YELLOW, YELLOW, 500, 0, 0);
    // Sphere sl3 = Sphere(l3.getLocation()+Vector{0,0.05,0}, 0.02, YELLOW, YELLOW, YELLOW, 500);
    
    // Retângulo Eixo X (VERMELHO)
    // Rectangle axisX = Rectangle({-1.7,-1,-2}, 0.03, 0.6, RED/8, RED, GRAY*1.3, 32, 0, 0);
    // Retângulo Eixo Y (VERDE)
    // Rectangle axisY = Rectangle({-1.7,-1,-2}, 0.03, 0.6, GREEN/8, GREEN, GRAY*1.3, 32, 0, 0);
    // Retângulo Eixo Z (AZUL)
    // Rectangle axisZ = Rectangle({-1.7,-1,-2}, 0.03, 0.6, BLUE/8, BLUE, GRAY*1.3, 32, 0, 0);

    // objList.push_back(&c1);
    // objList.push_back(&c2);
    objList.push_back(&s1);
    objList.push_back(&s2);
    objList.push_back(&s3);
    objList.push_back(&s4);
    // objList.push_back(&s5);
    objList.push_back(&p1);
    // objList.push_back(&t1);

    // objList.push_back(&sl1);
    // objList.push_back(&sl2);
    // objList.push_back(&sl3);

    // objList.push_back(&axisX);
    // objList.push_back(&axisZ);
    // objList.push_back(&axisY);
    // axisZ.rotateX(rad(90));
    // axisX.rotateZ(rad(90));
    // axisZ.transfer({-0.178,-0.39,-0.5});
    // axisX.transfer({0.285,-0.29,0});

    // for (int k = 0; k < objectFaces.size(); ++k) { 
    //     Point p0 = facePointsList[k][0];
    //     Point p1 = facePointsList[k][1];
    //     Point p2 = facePointsList[k][2];
    //     Triangle* temp = new Triangle(p0, p1, p2, objectFaces[k].ka, objectFaces[k].kd, objectFaces[k].ks, objectFaces[k].ns, 0.0, 1.0 - objectFaces[k].d);
    //     objList.push_back(temp);
    // }
    
    // Fim da lista de objetos
    // ----------------------------------------------------
    
    int prog = 1;
    exitRGB << "P3\n" << c.getHRes() << " " << c.getVRes() << "\n255\n";
    for (int j = c.getVRes() - 1; j >= 0; j--) { // de cima para baixo
        for (int i = 0; i < c.getHRes(); i++) { // da esquerda para direita
            // exibir_barra_de_loading(prog++, c.getVRes() * c.getHRes());
            exitRGB << paintScreen(i, j, c, objList, pontualLights, ambientLight) << "\n";
        }
    }

    exitRGB.close();
    cout << "Arquivo '" << fileName << ".ppm' criado na pasta 'img' com sucesso!" << endl;
    return 0;
}
