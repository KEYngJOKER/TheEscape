#pragma once
#include <string>
#include <map>
class Scene {
public:
    Scene(const std::string& name, const std::string& description, bool locked = false);
    void enter() const;
    std::string getName() const;
    bool isLocked() const;
    void unlock();

    std::map<std::string, bool> objects; //������������״̬
private:
    std::string name;
    std::string description;
    bool locked;
    
};
