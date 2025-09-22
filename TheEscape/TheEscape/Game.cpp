#include "Game.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// ���캯��
Game::Game() : attempts(0), isRunning(true) {
    // ��ʼ������
    scenes["��������"] = std::make_shared<Scene>("��������", "��վ���������룬�����һ�����ŵ����ţ��Ҳ��ǲ����������ʾ������ʾ���Ա�����һ��ֽ����ʾ�������ƺ���һ������");
    scenes["���������"] = std::make_shared<Scene>("���������", "��������������ң���ǰ��һ�����ϵ���ʾ������ʾ�������������ƺ���һ������������u�̣��ұ��ƺ���һ������", true);
    scenes["����ͨ��"] = std::make_shared<Scene>("����ͨ��", "��������ͨ���ľ�ͷ��ǰ���ƺ���һ�ȳ��ڴ���", true);

    currentScene = scenes["��������"];
    screenPuzzle = std::make_shared<Puzzle>("������4λ����:", "1024");
}

// ��Ϸ��ʼ
void Game::start() {
    std::cout << "��ӭ�������������δ�������ҡ�" << std::endl;
    std::cout << "������ start ��ʼ��Ϸ��help �鿴ָ�" << std::endl;
    std::string cmd;
    while (isRunning && std::getline(std::cin, cmd)) {
        processCommand(cmd);
    }
}

// ��������
void Game::processCommand(const std::string& cmd) {
    if (cmd.empty()) return;

    std::string lower = cmd;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "quit") {
        std::cout << "��Ϸ�������ټ���" << std::endl;
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
            handleUseKey(); // ʹ��key����
        else if (lower.find("drawer") != std::string::npos || lower.find("����") != std::string::npos)
            handleOpenObject(lower);
        else
            std::cout << "����û�п��Դ򿪵Ķ�����" << std::endl;
    }
    else if (lower == "use u��" || lower == "use u") {
        handleUseUsb();
    }
    else if (lower == "escape") {
        handleEscape();
    }
    else {
        std::cout << "��Чָ������� 'help' �鿴���ò�����" << std::endl;
    }
}

// �ƶ�����
void Game::handleGo(const std::string& cmd) {
    if (currentScene->getName() == "��������") {
        if (cmd.find("left") != std::string::npos) {
            if (!scenes["���������"]->isLocked()) {
                currentScene = scenes["���������"];
                currentScene->enter();
            }
            else {
                std::cout << "�������ţ���Ҫkey���ܽ��롣" << std::endl;
            }
        }
        else {
            std::cout << "��������߲�ͨ��" << std::endl;
        }
    }
    else if (currentScene->getName() == "���������") {
        if (cmd.find("right") != std::string::npos) {
            if (!scenes["����ͨ��"]->isLocked()) {
                currentScene = scenes["����ͨ��"];
                currentScene->enter();
            }
            else {
                std::cout << "ͨ��δ��������Ҫ���ƽ���ʾ����" << std::endl;
            }
        }
        else {
            std::cout << "��������߲�ͨ��" << std::endl;
        }
    }
    else {
        std::cout << "û������·�ˡ�" << std::endl;
    }
}

// �ռ�����
void Game::handleGet(const std::string& cmd) {
    if (currentScene->getName() == "��������" && cmd.find("key") != std::string::npos) {
        if (inventory["key"]) {
            std::cout << "���Ѿ�����������ˡ�" << std::endl;
        }
        else {
            inventory["key"] = true;
            std::cout << "�������һ�������key��" << std::endl;
        }
    }
    else if (currentScene->getName() == "��������" && cmd.find("paper") != std::string::npos) {
        if (inventory["paper"]) {
            std::cout << "���Ѿ�����������ˡ�" << std::endl;
        }
        else {
            inventory["paper"] = true;
            std::cout << "���ҵ���һ��д�����ֵ�paper��" << std::endl;
        }
    }
    else if (currentScene->getName() == "���������" && cmd.find("u��") != std::string::npos) {
        if (inventory["u��"]) {
            std::cout << "���Ѿ�����������ˡ�" << std::endl;
        }
        else {
            inventory["u��"] = true;
            std::cout << "���������������һ��U�̡�" << std::endl;
        }
    }
    else {
        std::cout << "����û��������ߡ�" << std::endl;
    }
}

// �鿴��Ʒ
void Game::handleLook(const std::string& cmd) {
    if (cmd.find("paper") != std::string::npos || cmd.find("paper") != std::string::npos) {
        if (inventory["paper"]) {
            std::cout << "paper�ϼ�¼���ĸ��������֣�" << std::endl;
            std::cout << "* * *" << std::endl;
            std::cout << "4 * *" << std::endl;
            std::cout << "1 3 *" << std::endl;
            std::cout << "* 2 *" << std::endl;
        }
        else {
            std::cout << "��û��paper��" << std::endl;
        }
    }
    else {
        std::cout << "û��ʲô�ر�ġ�" << std::endl;
    }
}

// �����߼�
void Game::handleOpenDoor() {
    if (currentScene->getName() == "��������") {
        if (inventory["key"]) {
            scenes["���������"]->unlock();
            std::cout << "����key�������ţ����Խ�����ߵķ��䡣" << std::endl;
        }
        else {
            std::cout << "�����ţ���Ҫkey��" << std::endl;
        }
    }
    else {
        std::cout << "����û������Ҫ�򿪡�" << std::endl;
    }
}

// ʹ��U�̣���ʾ�����⣩
void Game::handleUseUsb() {
    if (currentScene->getName() == "���������") {
        if (!inventory["u��"]) {
            std::cout << "��û��U�̡�" << std::endl;
            return;
        }
        std::cout << screenPuzzle->getQuestion() << std::endl;
        std::string input;
        std::getline(std::cin, input);

        if (screenPuzzle->checkAnswer(input)) {
            scenes["����ͨ��"]->unlock();
            std::cout << "������ȷ������ͨ���ѽ�����" << std::endl;
        }
        else {
            if (screenPuzzle->getAttempts() >= 3) {
                std::cout << "����������ʧ���ˣ�" << std::endl;
                isRunning = false;
            }
            else {
                std::cout << "������󣬻�ʣ " << (3 - screenPuzzle->getAttempts()) << " �λ��ᡣ" << std::endl;
            }
        }
    }
    else {
        std::cout << "���ﲻ����U�̡�" << std::endl;
    }
}

//ʹ��key����
void Game::handleUseKey() {
    if (currentScene->getName() == "��������" && inventory["key"]) {
        scenes["���������"]->unlock();
        std::cout << "��ʹ��key�����������ţ����Խ�����һ�䷿�䡣" << std::endl;
    }
    else {
        std::cout << "���ﲻ��ʹ��key�����㻹û��key��" << std::endl;
    }
}

// �����߼�
void Game::handleEscape() {
    if (currentScene->getName() == "����ͨ��") {
        std::cout << "��ɹ��ӳ����ң�������δ�������������ʸ�" << std::endl;
        isRunning = false;
    }
    else {
        std::cout << "���ﲻ�����ܡ�" << std::endl;
    }
}

void Game::handleOpenObject(const std::string& cmd) {
    if (currentScene->getName() == "��������") {
        if (cmd.find("drawer") != std::string::npos || cmd.find("����") != std::string::npos) {
            if (!currentScene->objects["drawer"]) {
                currentScene->objects["drawer"] = true;
                inventory["key"] = true; // ��һ��key
                std::cout << "����˳��룬����һ�������key�������뱳����" << std::endl;
            }
            else {
                std::cout << "�����Ѿ������ˡ�" << std::endl;
            }
        }
        else {
            std::cout << "����û�п��Դ򿪵Ķ�����" << std::endl;
        }
    }
    else {
        std::cout << "����û�п��Դ򿪵Ķ�����" << std::endl;
    }
}

// ��ʾ����
void Game::showHelp() const {
    std::cout << "����ָ��:\n"
        << "start - ��ʼ��Ϸ\n"
        << "go ���� - �ƶ����� (go left / go right)\n"
        << "get ������ - �ռ����� (key/paper/U��)\n"
        << "use ������ - ʹ�õ��� (U��)\n"
        << "look ��Ʒ - �鿴��Ʒ���� (paper)\n"
        << "open ���� - (drawer / door)\n"
        << "inventory - �鿴����\n"
        << "escape - ����\n"
        << "quit - �˳���Ϸ\n";
}

// ��ʾ����
void Game::showInventory() const {
    std::cout << "��ǰ������Ʒ:" << std::endl;
    if (inventory.empty()) {
        std::cout << "���գ�" << std::endl;
    }
    else {
        for (auto& pair : inventory) {
            std::cout << "- " << pair.first << std::endl;
        }
    }
}



