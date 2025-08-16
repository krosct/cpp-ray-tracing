#pragma once

#include "Thing.h"
#include "Point.h"
#include "Vector.h"

class Light : public Thing {
private:
    std::string lightName = ""; // Nome da luz
    Vector colour = {0,0,0}; // Cor da luz

public:
    Light(std::string lightName, Vector colour, std::string type) : Thing(type), lightName(lightName), colour(colour) {}
    virtual ~Light() = default;

    inline const Vector& getColour() const { return colour; }
    inline const std::string& getName() const { return lightName; }

    inline virtual Light* clone() const = 0;
};

// Classe para representar uma luz pontual
class PontualLight : public Light {
private:
    Point location;   // Localização da luz no espaço 3D
    
public:
    // Construtor
    PontualLight(std::string lightName, const Vector& colour, const Point& location) : Light(lightName, colour, "PontualLight"), location(location) {}

    // Getters
    inline const Point& getLocation() const { return location; }

    inline void print() const {
        std::cout << getType() << "(" << getName() << "): " << std::endl;
        std::cout << "colour: "; getColour().print();
        std::cout << "location: "; getLocation().print();
    }

    inline Light* clone() const { return new PontualLight(*this); }
};

// Classe para representar uma luz ambiente
class EnvLight : public Light {
public:
    // Construtor
    EnvLight(std::string lightName, const Vector& colour) : Light(lightName, colour, "EnvLight") {}

    inline Light* clone() const { return new EnvLight(*this); }
};

// Classe para representar uma luz extensa
class ExtensiveLight : public Light {
private:
    Point location;   // Localização da luz no espaço 3D
    unsigned numLights;
    std::vector<PontualLight*> lights;

public:
    // Construtor
    ExtensiveLight(std::string lightName, const Vector& colour, const Point& location, unsigned numLights = 1) : Light(lightName, colour, "ExtensiveLight"), location(location), numLights(numLights) {
        int side = static_cast<unsigned>(sqrt(numLights));
        for (int i = 0; i < side; ++i) {
            for (int j = 0; j < side; ++j) {
                lights.push_back(new PontualLight(lightName+"-Ext"+std::to_string(i)+std::to_string(j), colour, {location.getX()+j, location.getY(), location.getZ()+i}));
            }
        }
    }

    // Getters
    inline const Point& getLocation() const { return location; }
    inline unsigned getNumLights() const { return numLights; }
    inline const std::vector<PontualLight*>& getLights() const { return lights; }

    inline void print() const {
        std::cout << getType() << "(" << getName() << "): " << std::endl;
        std::cout << "colour: "; getColour().print();
        for (auto& l : lights) {
            std::cout << "location: "; l->getLocation().print();
        }
    }

    inline Light* clone() const { return new ExtensiveLight(*this); }
};