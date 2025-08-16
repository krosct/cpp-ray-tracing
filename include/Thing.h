#pragma once
#include <string>
class Thing {
private:
    std::string type;
public:
    Thing(std::string type) : type(type) {}
    inline const std::string& getType() const { return type; }
};
