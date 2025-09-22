#include "Game.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// 构造函数
Game::Game() : attempts(0), isRunning(true) {
    // 初始化场景
    scenes["密室中央"] = std::make_shared<Scene>("密室中央", "你站在密室中央，左侧有一扇锁着的铁门，右侧是布满代码的显示屏，显示屏旁边贴着一张纸，显示屏下面似乎有一个抽屉");
    scenes["代码控制室"] = std::make_shared<Scene>("代码控制室", "你来到代码控制室，眼前是一个古老的显示屏，显示屏下面桌子上似乎有一个遗留下来的u盘，右边似乎有一处暗门", true);
    scenes["逃生通道"] = std::make_shared<Scene>("逃生通道", "这是逃生通道的尽头，前方似乎有一扇出口大门", true);

    currentScene = scenes["密室中央"];
    screenPuzzle = std::make_shared<Puzzle>("请输入4位密码:", "1024");
}

// 游戏开始
void Game::start() {
    std::cout << "欢迎来到《逃脱虚空未来工作室》" << std::endl;
    std::cout << "请输入 start 开始游戏，help 查看指令。" << std::endl;
    std::string cmd;
    while (isRunning && std::getline(std::cin, cmd)) {
        processCommand(cmd);
    }
}

// 处理输入
void Game::processCommand(const std::string& cmd) {
    if (cmd.empty()) return;

    std::string lower = cmd;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "quit") {
        std::cout << "游戏结束，再见！" << std::endl;
        isRunning = false;
    }
    else if (lower == "help") {
        showHelp();
    }
    else if (lower == "inventory") {
        showInventory();
    }
    else if (lower == "start") {
        currentScene->enter();
    }
    else if (lower.rfind("go", 0) == 0) {
        handleGo(lower);
    }
    else if (lower.rfind("get", 0) == 0) {
        handleGet(lower);
    }
    else if (lower.rfind("look", 0) == 0) {
        handleLook(lower);
    }
    else if (lower.rfind("open", 0) == 0) {
        if (lower.find("door") != std::string::npos)
            handleUseKey(); // 使用key开门
        else if (lower.find("drawer") != std::string::npos || lower.find("抽屉") != std::string::npos)
            handleOpenObject(lower);
        else
            std::cout << "这里没有可以打开的东西。" << std::endl;
    }
    else if (lower == "use u盘" || lower == "use u") {
        handleUseUsb();
    }
    else if (lower == "escape") {
        handleEscape();
    }
    else {
        std::cout << "无效指令，请输入 'help' 查看可用操作。" << std::endl;
    }
}

// 移动场景
void Game::handleGo(const std::string& cmd) {
    if (currentScene->getName() == "密室中央") {
        if (cmd.find("left") != std::string::npos) {
            if (!scenes["代码控制室"]->isLocked()) {
                currentScene = scenes["代码控制室"];
                currentScene->enter();
            }
            else {
                std::cout << "铁门锁着，需要key才能进入。" << std::endl;
            }
        }
        else {
            std::cout << "这个方向走不通。" << std::endl;
        }
    }
    else if (currentScene->getName() == "代码控制室") {
        if (cmd.find("right") != std::string::npos) {
            if (!scenes["逃生通道"]->isLocked()) {
                currentScene = scenes["逃生通道"];
                currentScene->enter();
            }
            else {
                std::cout << "通道未解锁，需要先破解显示屏。" << std::endl;
            }
        }
        else {
            std::cout << "这个方向走不通。" << std::endl;
        }
    }
    else {
        std::cout << "没有其他路了。" << std::endl;
    }
}

// 收集道具
void Game::handleGet(const std::string& cmd) {
    if (currentScene->getName() == "密室中央" && cmd.find("key") != std::string::npos) {
        if (inventory["key"]) {
            std::cout << "你已经有这个道具了。" << std::endl;
        }
        else {
            inventory["key"] = true;
            std::cout << "你捡起了一把生锈的key。" << std::endl;
        }
    }
    else if (currentScene->getName() == "密室中央" && cmd.find("paper") != std::string::npos) {
        if (inventory["paper"]) {
            std::cout << "你已经有这个道具了。" << std::endl;
        }
        else {
            inventory["paper"] = true;
            std::cout << "你找到了一张写着数字的paper。" << std::endl;
        }
    }
    else if (currentScene->getName() == "代码控制室" && cmd.find("u盘") != std::string::npos) {
        if (inventory["u盘"]) {
            std::cout << "你已经有这个道具了。" << std::endl;
        }
        else {
            inventory["u盘"] = true;
            std::cout << "你从桌子上拿起了一个U盘。" << std::endl;
        }
    }
    else {
        std::cout << "这里没有这个道具。" << std::endl;
    }
}

// 查看物品
void Game::handleLook(const std::string& cmd) {
    if (cmd.find("paper") != std::string::npos || cmd.find("paper") != std::string::npos) {
        if (inventory["paper"]) {
            std::cout << "paper上记录了四个神秘数字：" << std::endl;
            std::cout << "* * *" << std::endl;
            std::cout << "4 * *" << std::endl;
            std::cout << "1 3 *" << std::endl;
            std::cout << "* 2 *" << std::endl;
        }
        else {
            std::cout << "你没有paper。" << std::endl;
        }
    }
    else {
        std::cout << "没有什么特别的。" << std::endl;
    }
}

// 开门逻辑
void Game::handleOpenDoor() {
    if (currentScene->getName() == "密室中央") {
        if (inventory["key"]) {
            scenes["代码控制室"]->unlock();
            std::cout << "你用key打开了铁门，可以进入左边的房间。" << std::endl;
        }
        else {
            std::cout << "门锁着，需要key。" << std::endl;
        }
    }
    else {
        std::cout << "这里没有门需要打开。" << std::endl;
    }
}

// 使用U盘（显示屏谜题）
void Game::handleUseUsb() {
    if (currentScene->getName() == "代码控制室") {
        if (!inventory["u盘"]) {
            std::cout << "你没有U盘。" << std::endl;
            return;
        }
        std::cout << screenPuzzle->getQuestion() << std::endl;
        std::string input;
        std::getline(std::cin, input);

        if (screenPuzzle->checkAnswer(input)) {
            scenes["逃生通道"]->unlock();
            std::cout << "密码正确！逃生通道已解锁。" << std::endl;
        }
        else {
            if (screenPuzzle->getAttempts() >= 3) {
                std::cout << "警报响起，你失败了！" << std::endl;
                isRunning = false;
            }
            else {
                std::cout << "密码错误，还剩 " << (3 - screenPuzzle->getAttempts()) << " 次机会。" << std::endl;
            }
        }
    }
    else {
        std::cout << "这里不能用U盘。" << std::endl;
    }
}

//使用key开门
void Game::handleUseKey() {
    if (currentScene->getName() == "密室中央" && inventory["key"]) {
        scenes["代码控制室"]->unlock();
        std::cout << "你使用key打开了左侧的铁门，可以进入下一间房间。" << std::endl;
    }
    else {
        std::cout << "这里不能使用key或者你还没有key。" << std::endl;
    }
}

// 逃脱逻辑
void Game::handleEscape() {
    if (currentScene->getName() == "逃生通道") {
        std::cout << "你成功逃出密室，获得虚空未来工作室面试资格！" << std::endl;
        isRunning = false;
    }
    else {
        std::cout << "这里不能逃跑。" << std::endl;
    }
}

void Game::handleOpenObject(const std::string& cmd) {
    if (currentScene->getName() == "密室中央") {
        if (cmd.find("drawer") != std::string::npos || cmd.find("抽屉") != std::string::npos) {
            if (!currentScene->objects["drawer"]) {
                currentScene->objects["drawer"] = true;
                inventory["key"] = true; // 玩家获得key
                std::cout << "你打开了抽屉，发现一把生锈的key，并放入背包。" << std::endl;
            }
            else {
                std::cout << "抽屉已经被打开了。" << std::endl;
            }
        }
        else {
            std::cout << "这里没有可以打开的东西。" << std::endl;
        }
    }
    else {
        std::cout << "这里没有可以打开的东西。" << std::endl;
    }
}

// 显示帮助
void Game::showHelp() const {
    std::cout << "可用指令:\n"
        << "start - 开始游戏\n"
        << "go 方向 - 移动场景 (go left / go right)\n"
        << "get 道具名 - 收集道具 (key/paper/U盘)\n"
        << "use 道具名 - 使用道具 (U盘)\n"
        << "look 物品 - 查看物品详情 (paper)\n"
        << "open 道具 - (drawer / door)\n"
        << "inventory - 查看背包\n"
        << "escape - 逃生\n"
        << "quit - 退出游戏\n";
}

// 显示背包
void Game::showInventory() const {
    std::cout << "当前背包物品:" << std::endl;
    if (inventory.empty()) {
        std::cout << "（空）" << std::endl;
    }
    else {
        for (auto& pair : inventory) {
            std::cout << "- " << pair.first << std::endl;
        }
    }
}



