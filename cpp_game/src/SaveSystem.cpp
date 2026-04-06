#include "SaveSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>

SaveSystem::SaveSystem() : currentLevel(0), totalWins(0), totalPlayTime(0) {
    saveFilePath = "savegame.dat";
    loadProgress();
}

SaveSystem::~SaveSystem() {
    saveProgress();
}

void SaveSystem::saveProgress() {
    std::ofstream file(saveFilePath, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
        file.write(reinterpret_cast<char*>(&totalWins), sizeof(totalWins));
        file.write(reinterpret_cast<char*>(&totalPlayTime), sizeof(totalPlayTime));

        // Save unlocked characters
        size_t charCount = unlockedCharacters.size();
        file.write(reinterpret_cast<char*>(&charCount), sizeof(charCount));
        for (const auto& pair : unlockedCharacters) {
            size_t nameSize = pair.first.size();
            file.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            file.write(pair.first.c_str(), nameSize);
            file.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        // Save achievements
        size_t achCount = achievements.size();
        file.write(reinterpret_cast<char*>(&achCount), sizeof(achCount));
        for (const auto& pair : achievements) {
            size_t nameSize = pair.first.size();
            file.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            file.write(pair.first.c_str(), nameSize);
            file.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        file.close();
    }
}

bool SaveSystem::loadProgress() {
    std::ifstream file(saveFilePath, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
        file.read(reinterpret_cast<char*>(&totalWins), sizeof(totalWins));
        file.read(reinterpret_cast<char*>(&totalPlayTime), sizeof(totalPlayTime));

        // Load unlocked characters
        size_t charCount;
        file.read(reinterpret_cast<char*>(&charCount), sizeof(charCount));
        for (size_t i = 0; i < charCount; ++i) {
            size_t nameSize;
            file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            std::string name(nameSize, '\0');
            file.read(&name[0], nameSize);
            bool unlocked;
            file.read(reinterpret_cast<char*>(&unlocked), sizeof(unlocked));
            unlockedCharacters[name] = unlocked;
        }

        // Load achievements
        size_t achCount;
        file.read(reinterpret_cast<char*>(&achCount), sizeof(achCount));
        for (size_t i = 0; i < achCount; ++i) {
            size_t nameSize;
            file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            std::string name(nameSize, '\0');
            file.read(&name[0], nameSize);
            bool unlocked;
            file.read(reinterpret_cast<char*>(&unlocked), sizeof(unlocked));
            achievements[name] = unlocked;
        }

        file.close();
        return true;
    }
    return false;
}

void SaveSystem::setCurrentLevel(int level) {
    currentLevel = level;
}

int SaveSystem::getCurrentLevel() const {
    return currentLevel;
}

void SaveSystem::unlockCharacter(const std::string& character) {
    unlockedCharacters[character] = true;
}

bool SaveSystem::isCharacterUnlocked(const std::string& character) const {
    auto it = unlockedCharacters.find(character);
    return it != unlockedCharacters.end() && it->second;
}

void SaveSystem::unlockAchievement(const std::string& achievement) {
    achievements[achievement] = true;
}

bool SaveSystem::isAchievementUnlocked(const std::string& achievement) const {
    auto it = achievements.find(achievement);
    return it != achievements.end() && it->second;
}

void SaveSystem::addWin() {
    totalWins++;
}

int SaveSystem::getTotalWins() const {
    return totalWins;
}

void SaveSystem::addPlayTime(int seconds) {
    totalPlayTime += seconds;
}

int SaveSystem::getTotalPlayTime() const {
    return totalPlayTime;
}

std::string SaveSystem::exportSaveData() const {
    std::stringstream ss;
    ss << "Level:" << currentLevel << "\n";
    ss << "Wins:" << totalWins << "\n";
    ss << "PlayTime:" << totalPlayTime << "\n";
    ss << "Characters:";
    for (const auto& pair : unlockedCharacters) {
        if (pair.second) ss << pair.first << ",";
    }
    ss << "\nAchievements:";
    for (const auto& pair : achievements) {
        if (pair.second) ss << pair.first << ",";
    }
    return ss.str();
}

bool SaveSystem::importSaveData(const std::string& data) {
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            if (key == "Level") currentLevel = std::stoi(value);
            else if (key == "Wins") totalWins = std::stoi(value);
            else if (key == "PlayTime") totalPlayTime = std::stoi(value);
        }
    }
    return true;
}