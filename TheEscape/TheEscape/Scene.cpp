#include "Scene.h"
#include <iostream>

Scene::Scene(const std::string& name, const std::string& description, bool locked)
    : name(name), description(description), locked(locked) 
{
    if (name == "��������") {
        objects["drawer"] = false; // ����δ��
    }
}

void Scene::enter() const {
    std::cout << "��" << name << "�� " << description << std::endl;
}

std::string Scene::getName() const {
    return name;
}

bool Scene::isLocked() const {
    return locked;
}

void Scene::unlock() {
    locked = false;
}

