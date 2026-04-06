#ifndef STORY_MODE_H
#define STORY_MODE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Level.h"
#include "SaveSystem.h"

class StoryMode {
private:
    std::vector<Level> levels;
    int currentLevel;
    SaveSystem saveSystem;
    std::string currentDialogue;
    sf::Clock dialogueTimer;
    bool showingDialogue;

public:
    StoryMode();
    ~StoryMode();

    void initialize();
    void update(float deltaTime);
    void render(sf::RenderWindow& window, sf::Font& font);
    bool isComplete() const;
    Level& getCurrentLevel();
    void nextLevel();
    void showDialogue(const std::string& text);
    bool isShowingDialogue() const;
    void skipDialogue();

    // Story progression
    void handleStoryEvents();
    std::string getCurrentStoryText() const;
};

#endif // STORY_MODE_H