#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Particle.h"
#include "AudioManager.h"
#include "Menu.h"
#include "StoryMode.h"
#include "StoryManager.h"
#include "SaveSystem.h"
#include "AnimationManager.h"
#include "SpriteGenerator.h"

enum class GameState {
    MENU,
    STORY_MODE,
    ARCADE_MODE,
    SURVIVAL_MODE,
    VERSUS_MODE,
    TRAINING_MODE,
    CHARACTER_SELECT,
    LEVEL_SELECT,
    SETTINGS,
    ACHIEVEMENTS,
    CUTSCENE,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::unique_ptr<Player> player;
    std::unique_ptr<Enemy> enemy;
    ParticleSystem particles;
    AudioManager audioManager;
    Menu mainMenu;
    Menu characterMenu;
    Menu modeMenu;
    std::unique_ptr<StoryMode> storyMode;
    std::unique_ptr<StoryManager> storyManager;
    SaveSystem saveSystem;
    AnimationManager animationManager;
    SpriteGenerator spriteGenerator;
    sf::Clock clock;
    sf::Clock gameClock;
    GameState gameState;
    GameState previousState;
    int wins;
    float gameTime;
    std::string selectedCharacter;
    std::string selectedEnemy;

    // UI elements
    sf::Text fpsText;
    sf::Text versionText;

public:
    Game();
    ~Game();

    void run();
    void processEvents();
    void update(float deltaTime);
    void render();
    void resetGame();
    bool checkAttackCollision(Fighter* attacker, Fighter* target);
    void drawBackground();
    void drawUI();
    void drawMenu();
    void drawCharacterSelect();
    void drawModeSelect();
    void drawPauseMenu();
    void drawGameOver();
    void drawAchievements();
    void drawSettings();
    void drawCutscene();

    // State management
    void changeState(GameState newState);
    void startGameMode(Constants::GameMode mode);

    // Character management
    void createCharacter(const std::string& name, bool isPlayer = true);
    void loadCharacterData();

    // Utility
    void updateFPS(float deltaTime);
    void handleAchievements();
};

#endif // GAME_H