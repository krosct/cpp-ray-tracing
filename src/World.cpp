#include "../include/World.h"

using namespace std;

unsigned World::numObjs = 0;
unsigned World::numLights = 0;
unsigned World::numCameras = 0;

// Construtores
World::World() : envLight(EnvLight("", {0, 0, 0})) {}

// Print do mundo
void World::print() const {
    cout << "Object List of the World:" << endl;
    cout << "~~~" << endl;
    unsigned i = 0;
    for (auto& obj : getObjList()) {
        cout << i++ << ". (" << obj.second->getType() << " - " << obj.second->getName() << ")" << ":" << endl;
        obj.second->print();
    }
    cout << "~~~" << endl;
    cout << "Light List of the World:" << endl;
    i = 0;
    for (auto& light : getPontualLightList()) {
        cout << i++ << ". (" << light.second->getType() << " - " << light.second->getName() << ")" << ":" << endl;
        light.second->print();
    }
    cout << "~~~" << endl;
    cout << "Camera List of the World:" << endl;
    i = 0;
    for (auto& camera : getCameraList()) {
        cout << i++ << ". (" << camera.second->getType() << " - " << camera.second->getName() << ")" << ":" << endl;
        camera.second->print();
    }
    cout << "~~~" << endl;
}

// Getters
const unordered_map<string, Hittable*>& World::getObjList() const { return objList; }
const unordered_map<string, ExtensiveLight*>& World::getPontualLightList() const { return lightList; }
const unordered_map<string, Camera*>& World::getCameraList() const { return cameraList; }
const EnvLight& World::getEnvLight() const { return envLight; }
const unsigned& World::getnumObjs() const { return numObjs; }
const unsigned& World::getnumLights() const { return numLights; }
const unsigned& World::getnumCameras() const { return numCameras; }

void World::add(Cube obj) {
    if (objList.count(obj.getName())) {
        cerr << "ERROR: Cube already exists!" << endl;
        return;
    }
    if (obj.getName() == "") {
        std::cerr << "ERROR: cube MUST have a name" << std::endl;
        return;
    }
    objList.insert(pair<string, Hittable*>(obj.getName(), new Cube(obj)));
}

void World::add(Sphere obj) {
    if (objList.count(obj.getName())) {
        cerr << "ERROR: Sphere already exists!" << endl;
        return;
    }
    if (obj.getName() == "") {
        std::cerr << "ERROR: sphere MUST have a name" << std::endl;
        return;
    }
    objList.insert(pair<string, Hittable*>(obj.getName(), new Sphere(obj)));
}

void World::add(Plane obj) {
    if (objList.count(obj.getName())) {
        cerr << "ERROR: Plane already exists!" << endl;
        return;
    }
    if (obj.getName() == "") {
        std::cerr << "ERROR: plane MUST have a name" << std::endl;
        return;
    }
    objList.insert(pair<string, Hittable*>(obj.getName(), new Plane(obj)));
}

void World::add(Triangle obj) {
    if (objList.count(obj.getName())) {
        cerr << "ERROR: Triangle already exists!" << endl;
        return;
    }
    if (obj.getName() == "") {
        std::cerr << "ERROR: triangle MUST have a name" << std::endl;
        return;
    }
    objList.insert(pair<string, Hittable*>(obj.getName(), new Triangle(obj)));
}

void World::add(const Hittable& obj) {
    string objType = obj.getType();
    
    if (objList.count(obj.getName())) {
        cerr << "ERROR: " << objType << " already exists!" << endl;
        return;
    }
    if (obj.getName() == "") {
        std::cerr << "ERROR: " << objType << " MUST have a name" << std::endl;
        return;
    }

    objList.insert(make_pair(obj.getName(), obj.clone()));
}

// void World::add(PontualLight light, bool guide) {
//     if (lightList.count(light.getName())) {
//         cerr << "ERROR: a PontualLight already exists!" << endl;
//         return;
//     }
//     if (light.getName() == "") {
//         std::cerr << "ERROR: pontual light MUST have a name" << std::endl;
//         return;
//     }
//     lightList.insert(pair<string, PontualLight*>(light.getName(), new PontualLight(light)));
//     if (guide) {
//         string lightName = light.getName();
//         add(Sphere("guideOf:"+lightName,getPontualLightList().find(lightName)->second->getLocation()+Vector(0,0.05,0),0.05,{255,255,0},{255,255,0},{255,255,0},1000,0,0));
//     }
// }

void World::add(EnvLight newLight){
    if (envLight.getName() != "") {
        std::cerr << "ERROR: a EnvLight already exists!" << std::endl;
        return;
    }
    if (newLight.getName() == "") {
        std::cerr << "ERROR: env light MUST have a name" << std::endl;
        return;
    }
    envLight = newLight;
}

void World::add(ExtensiveLight light, bool guide) {
    if (lightList.count(light.getName())) {
        cerr << "ERROR: a ExtensiveLight already exists!" << endl;
        return;
    }
    if (light.getName() == "") {
        std::cerr << "ERROR: pontual light MUST have a name" << std::endl;
        return;
    }
    lightList.insert(make_pair(light.getName(), new ExtensiveLight(light)));
    if (guide) {
        string lightName = light.getName();
        unsigned count = 0;
        for (auto& l : light.getLights()) {
            add(Sphere(
                "guideOf:"+lightName+to_string(count++),
                l->getLocation()+Vector(0,0.05,0),
                0.05,
                {255,255,0},{255,255,0},{255,255,0},1000,0,0));
        }
    }
}

void World::rem(const Hittable& obj) {
    if (objList.count(obj.getName())) {
        delete objList[obj.getName()];
        objList.erase(obj.getName());
    }
}

void World::add(Camera camera) {
    if (cameraList.count(camera.getName())) {
        cerr << "ERROR: Camera already exists!" << endl;
        return;
    }
    if (camera.getName() == "") {
        std::cerr << "ERROR: camera MUST have a name" << std::endl;
        return;
    }
    cameraList.insert(make_pair(camera.getName(), new Camera(camera)));
}

void World::rem(string thingName) {
    if (objList.count(thingName)) {
        delete objList[thingName];
        objList.erase(thingName);
        return;
    }
    if (lightList.count(thingName)) {
        delete lightList[thingName];
        lightList.erase(thingName);
        return;
    }
    if (cameraList.count(thingName)) {
        delete cameraList[thingName];
        cameraList.erase(thingName);
        return;
    }
    if (envLight.getName() == thingName) {
        rem();
        return;
    }
}

void World::rem(PontualLight light) {
    if (lightList.count(light.getName())) {
        delete lightList[light.getName()];
        lightList.erase(light.getName());
    }
}

void World::rem() {
    envLight = EnvLight("", {0,0,0}); 
}

void World::rem(Camera camera) {
    if (cameraList.count(camera.getName())) {
        delete cameraList[camera.getName()];
        cameraList.erase(camera.getName());
    }
}

void World::buildOctree() {
    std::vector<Hittable*> objs;
    // Filtra objetos problemáticos
    for (auto const& [key, obj] : objList) {
        if (obj->getBoundingBox().min.getX() != numeric_limits<double>::max()) {
            objs.push_back(obj);
        }
        // Todo: Adicionar quaisquer objetos que não possam ser
        // Todo: divididos nas bounding boxes
    }
    octree = new Octree(objs);
}

const Octree* World::getOctree() const {
    return octree;
}