#pragma once
#include <string>

class Puzzle {
public:
    Puzzle(const std::string& question, const std::string& answer);
    std::string getQuestion() const;
    bool checkAnswer(const std::string& input);
    int getAttempts() const;

private:
    std::string question;
    std::string answer;
    int attempts;
};
