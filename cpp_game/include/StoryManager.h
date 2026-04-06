#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Cutscene.h"
#include "Level.h"
#include "SpriteGenerator.h"

class StoryManager {
private:
    std::vector<Cutscene*> cutscenes;
    std::vector<Level*> levels;
    int currentAct;
    int currentScene;
    SpriteGenerator spriteGen;
    sf::Font font;

    // Story progression
    enum class StoryPhase {
        PROLOGUE,
        ACT1_SCHOOL,
        ACT2_CONFLICT,
        ACT3_RESOLUTION,
        EPILOGUE
    };
    StoryPhase currentPhase;

public:
    StoryManager(sf::Font& f);
    ~StoryManager();

    void initializeStory();
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isStoryComplete() const;
    Level* getCurrentLevel();
    Cutscene* getCurrentCutscene();

    // Story control
    void nextScene();
    void startAct(int act);
    std::string getCurrentStoryText() const;

private:
    void createPrologue();
    void createAct1();
    void createAct2();
    void createAct3();
    void createEpilogue();

    void setupCharacterSprites();
    void createStoryLevels();
};

#endif // STORY_MANAGER_H