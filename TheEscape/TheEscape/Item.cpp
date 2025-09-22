#include "Item.h"

Item::Item(const std::string& n, const std::string& desc)
    : name(n), description(desc) {}

std::string Item::getName() const {
    return name;
}

std::string Item::getDescription() const {
    return description;
}
