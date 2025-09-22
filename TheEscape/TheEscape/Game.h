#pragma once
#include <string>
#include <map>
#include <memory>
#include "Scene.h"
#include "Puzzle.h"

class Game {
public:
    Game();
    void start();

private:
    std::map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> currentScene;
    std::map<std::string, bool> inventory;
    std::shared_ptr<Puzzle> screenPuzzle;
    int attempts;
    bool isRunning;

    void processCommand(const std::string& cmd);

    void handleGo(const std::string& cmd);
    void handleGet(const std::string& cmd);
    void handleLook(const std::string& cmd);
    void handleOpenDoor();
    void handleUseUsb();
    void handleUseKey();
    void handleEscape();
    void handleOpenObject(const std::string& cmd);

    void showHelp() const;
    void showInventory() const;
};
