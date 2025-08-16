#pragma once
#include "Operations.h"
#include <vector>
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Cube.h"
#include "Hittable.h"
#include "Light.h"
#include "Camera.h"
#include <unordered_map>
#include "Octree.h"

class World{
private:
    std::unordered_map<std::string, Hittable*> objList;
    std::unordered_map<std::string, ExtensiveLight*> lightList;
    std::unordered_map<std::string, Camera*> cameraList;
    EnvLight envLight;
    Octree* octree;
    static unsigned numObjs;
    static unsigned numLights;
    static unsigned numCameras;

public:
    // Construtores
    World();
    ~World();

    // Print do mundo
    void print() const;

    // Getters
    const std::unordered_map<std::string, Hittable*>& getObjList() const;
    const std::unordered_map<std::string, ExtensiveLight*>& getPontualLightList() const;
    const std::unordered_map<std::string, Camera*>& getCameraList() const;
    const EnvLight& getEnvLight() const;
    const unsigned& getnumObjs() const;
    const unsigned& getnumLights() const;
    const unsigned& getnumCameras() const;

    // Add & Remove OBJECTS
    void add(const Hittable& obj);
    void add(Cube obj);
    void add(Sphere obj);
    void add(Plane obj);
    void add(Triangle obj);
    void add(PontualLight light, bool guide = false);
    void add(ExtensiveLight, bool guide = false);
    void add(Camera camera);
    void add(EnvLight newLight);
    
    void rem(const Hittable& obj);
    void rem(PontualLight light);
    void rem(std::string lightName);
    void rem();
    void rem(Camera camera);

    void buildOctree();
    const Octree* getOctree() const;
};
