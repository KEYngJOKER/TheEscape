#include "Puzzle.h"

Puzzle::Puzzle(const std::string& question, const std::string& answer)
    : question(question), answer(answer), attempts(0) {}

std::string Puzzle::getQuestion() const {
    return question;
}

bool Puzzle::checkAnswer(const std::string& input) {
    attempts++;
    return input == answer;
}

int Puzzle::getAttempts() const {
    return attempts;
}
