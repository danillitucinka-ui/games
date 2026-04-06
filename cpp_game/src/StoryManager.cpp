#include "StoryManager.h"

StoryManager::StoryManager(sf::Font& f) : font(f), currentAct(0), currentScene(0), currentPhase(StoryPhase::PROLOGUE) {
    initializeStory();
}

StoryManager::~StoryManager() {
    for (auto cutscene : cutscenes) delete cutscene;
    for (auto level : levels) delete level;
}

void StoryManager::initializeStory() {
    setupCharacterSprites();
    createStoryLevels();
    createPrologue();
    createAct1();
    createAct2();
    createAct3();
    createEpilogue();
}

void StoryManager::setupCharacterSprites() {
    // Generate character sprites
    spriteGen.generateCharacterSprite("Koushi Shirota", Constants::SHIROTA_COLOR,
                                    sf::Color(42, 42, 74), sf::Color(100, 149, 237), "normal");
    spriteGen.generateCharacterSprite("Takane Takamine", Constants::TAKAMINE_COLOR,
                                    sf::Color(106, 42, 74), sf::Color(220, 20, 60), "tsundere");
    spriteGen.generateCharacterSprite("Miki Sato", sf::Color(255, 200, 100),
                                    sf::Color(139, 69, 19), sf::Color(34, 139, 34), "cheerful");
    spriteGen.generateCharacterSprite("Seiya Ouji", sf::Color(150, 150, 255),
                                    sf::Color(25, 25, 112), sf::Color(70, 130, 180), "mysterious");
    spriteGen.generateCharacterSprite("Kiriko Shiratori", sf::Color(255, 150, 200),
                                    sf::Color(139, 0, 139), sf::Color(255, 0, 255), "elegant");
}

void StoryManager::createStoryLevels() {
    // Create levels for story progression
    levels.push_back(new Level(Constants::LEVELS[0])); // School Rooftop
    levels.push_back(new Level(Constants::LEVELS[1])); // Classroom Chaos
    levels.push_back(new Level(Constants::LEVELS[2])); // School Garden
    levels.push_back(new Level(Constants::LEVELS[3])); // Night City
    levels.push_back(new Level(Constants::LEVELS[4])); // Abandoned Warehouse
    levels.push_back(new Level(Constants::LEVELS[5])); // Mountain Summit
}

void StoryManager::createPrologue() {
    Cutscene* prologue = new Cutscene(font);

    // Opening scene
    prologue->addCharacter(spriteGen.getTexture("Koushi Shirota_normal"), sf::Vector2f(200, 200));
    prologue->addDialogue("Narrator", "In the quiet town of... well, this town... lives a young student named Koushi Shirota.");
    prologue->addDialogue("Koushi Shirota", "Another peaceful day at school... I wonder what adventures await?");
    prologue->addDialogue("Narrator", "Little does he know that today will change everything...");

    cutscenes.push_back(prologue);
}

void StoryManager::createAct1() {
    Cutscene* act1_intro = new Cutscene(font);

    act1_intro->addCharacter(spriteGen.getTexture("Koushi Shirota_normal"), sf::Vector2f(150, 200));
    act1_intro->addCharacter(spriteGen.getTexture("Takane Takamine_tsundere"), sf::Vector2f(400, 200));
    act1_intro->addDialogue("Takane Takamine", "Hmph! What are you staring at, Shirota?");
    act1_intro->addDialogue("Koushi Shirota", "I-I'm sorry! I didn't mean to...");
    act1_intro->addDialogue("Takane Takamine", "Don't think you can get close to me just because we're in the same class!");
    act1_intro->addDialogue("Koushi Shirota", "R-right! I understand!");
    act1_intro->addDialogue("Narrator", "Thus begins their story... a tale of misunderstanding and growing feelings.");

    cutscenes.push_back(act1_intro);

    // More act 1 scenes...
}

void StoryManager::createAct2() {
    Cutscene* act2_conflict = new Cutscene(font);

    act2_conflict->addCharacter(spriteGen.getTexture("Miki Sato_cheerful"), sf::Vector2f(100, 200));
    act2_conflict->addCharacter(spriteGen.getTexture("Seiya Ouji_mysterious"), sf::Vector2f(300, 200));
    act2_conflict->addCharacter(spriteGen.getTexture("Kiriko Shiratori_elegant"), sf::Vector2f(500, 200));
    act2_conflict->addDialogue("Miki Sato", "Hey everyone! Let's all get along!");
    act2_conflict->addDialogue("Seiya Ouji", "The winds of change are blowing... Can you feel it?");
    act2_conflict->addDialogue("Kiriko Shiratori", "Some relationships are destined to be complicated...");
    act2_conflict->addDialogue("Narrator", "As friendships form and conflicts arise, our protagonists must face their true feelings.");

    cutscenes.push_back(act2_conflict);
}

void StoryManager::createAct3() {
    Cutscene* act3_climax = new Cutscene(font);

    act3_climax->addCharacter(spriteGen.getTexture("Takane Takamine_tsundere"), sf::Vector2f(300, 200));
    act3_climax->addCharacter(spriteGen.getTexture("Koushi Shirota_normal"), sf::Vector2f(500, 200));
    act3_climax->addDialogue("Takane Takamine", "Why do you keep following me around?!");
    act3_climax->addDialogue("Koushi Shirota", "Because... because I care about you!");
    act3_climax->addDialogue("Takane Takamine", "W-what?! You... you idiot!");
    act3_climax->addDialogue("Narrator", "In the heat of battle, true feelings are revealed...");

    cutscenes.push_back(act3_climax);
}

void StoryManager::createEpilogue() {
    Cutscene* epilogue = new Cutscene(font);

    epilogue->addCharacter(spriteGen.getTexture("Koushi Shirota_normal"), sf::Vector2f(200, 200));
    epilogue->addCharacter(spriteGen.getTexture("Takane Takamine_tsundere"), sf::Vector2f(400, 200));
    epilogue->addDialogue("Narrator", "And so, after many trials and misunderstandings...");
    epilogue->addDialogue("Takane Takamine", "Hmph... I guess you're not completely useless, Shirota.");
    epilogue->addDialogue("Koushi Shirota", "Takamine... does this mean...?");
    epilogue->addDialogue("Takane Takamine", "D-don't get the wrong idea! This doesn't mean I like you or anything!");
    epilogue->addDialogue("Narrator", "Their story continues... in the most tsundere way possible.");
    epilogue->addDialogue("Narrator", "The End... or is it just the beginning?");

    cutscenes.push_back(epilogue);
}

void StoryManager::update(float deltaTime) {
    if (currentScene < static_cast<int>(cutscenes.size())) {
        cutscenes[currentScene]->update(deltaTime);
        if (cutscenes[currentScene]->isFinished()) {
            nextScene();
        }
    } else if (currentAct < static_cast<int>(levels.size())) {
        levels[currentAct]->update(deltaTime);
    }
}

void StoryManager::render(sf::RenderWindow& window) {
    if (currentScene < static_cast<int>(cutscenes.size())) {
        cutscenes[currentScene]->render(window);
    } else if (currentAct < static_cast<int>(levels.size())) {
        levels[currentAct]->render(window, font);
    }
}

bool StoryManager::isStoryComplete() const {
    return currentAct >= static_cast<int>(levels.size()) &&
           currentScene >= static_cast<int>(cutscenes.size());
}

Level* StoryManager::getCurrentLevel() {
    if (currentAct < static_cast<int>(levels.size())) {
        return levels[currentAct];
    }
    return nullptr;
}

Cutscene* StoryManager::getCurrentCutscene() {
    if (currentScene < static_cast<int>(cutscenes.size())) {
        return cutscenes[currentScene];
    }
    return nullptr;
}

void StoryManager::nextScene() {
    currentScene++;
    if (currentScene >= static_cast<int>(cutscenes.size()) && currentAct < static_cast<int>(levels.size())) {
        // Start next act
        currentAct++;
        currentScene = 0; // Reset for potential act-specific cutscenes
    }
}

void StoryManager::startAct(int act) {
    currentAct = act;
    currentScene = 0;
}

std::string StoryManager::getCurrentStoryText() const {
    if (currentScene < static_cast<int>(cutscenes.size())) {
        return cutscenes[currentScene]->isFinished() ? "" : "Cutscene in progress...";
    }
    return "Level " + std::to_string(currentAct + 1) + " in progress...";
}