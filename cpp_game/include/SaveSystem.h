#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include <map>
#include <vector>

class SaveSystem {
private:
    std::string saveFilePath;
    int currentLevel;
    std::map<std::string, bool> unlockedCharacters;
    std::map<std::string, bool> achievements;
    int totalWins;
    int totalPlayTime;

public:
    SaveSystem();
    ~SaveSystem();

    void saveProgress();
    bool loadProgress();

    void setCurrentLevel(int level);
    int getCurrentLevel() const;

    void unlockCharacter(const std::string& character);
    bool isCharacterUnlocked(const std::string& character) const;

    void unlockAchievement(const std::string& achievement);
    bool isAchievementUnlocked(const std::string& achievement) const;

    void addWin();
    int getTotalWins() const;

    void addPlayTime(int seconds);
    int getTotalPlayTime() const;

    // Data export/import
    std::string exportSaveData() const;
    bool importSaveData(const std::string& data);
};

#endif // SAVE_SYSTEM_H