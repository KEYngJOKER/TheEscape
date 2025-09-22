#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
    std::string name;
    std::string description;

public:
    Item(const std::string& n, const std::string& desc);
    std::string getName() const;
    std::string getDescription() const;
};

#endif
