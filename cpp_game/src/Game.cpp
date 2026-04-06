#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT), Constants::WINDOW_TITLE),
      player(nullptr), enemy(nullptr), mainMenu(font), characterMenu(font), modeMenu(font),
      storyMode(nullptr), storyManager(nullptr), gameState(GameState::MENU), previousState(GameState::MENU), wins(0), gameTime(0) {

    // Load font
    if (!font.loadFromFile(Constants::FONT_PATH)) {
        // Using default font
    }

    // Setup main menu
    mainMenu.setTitle("SHIROTA: BLOOD POLARITY ULTIMATE", sf::Vector2f(200, 150));
    mainMenu.addItem("Story Mode", sf::Vector2f(450, 300));
    mainMenu.addItem("Arcade Mode", sf::Vector2f(450, 350));
    mainMenu.addItem("Survival Mode", sf::Vector2f(450, 400));
    mainMenu.addItem("Versus Mode", sf::Vector2f(450, 450));
    mainMenu.addItem("Training", sf::Vector2f(450, 500));
    mainMenu.addItem("Achievements", sf::Vector2f(450, 550));
    mainMenu.addItem("Settings", sf::Vector2f(450, 600));
    mainMenu.addItem("Exit", sf::Vector2f(450, 650));

    // Setup mode menu
    modeMenu.setTitle("SELECT MODE", sf::Vector2f(400, 200));
    modeMenu.addItem("Easy", sf::Vector2f(450, 350));
    modeMenu.addItem("Normal", sf::Vector2f(450, 400));
    modeMenu.addItem("Hard", sf::Vector2f(450, 450));
    modeMenu.addItem("Expert", sf::Vector2f(450, 500));

    // Setup character menu
    characterMenu.setTitle("SELECT CHARACTER", sf::Vector2f(350, 200));
    for (const auto& pair : Constants::CHARACTERS) {
        characterMenu.addItem(pair.first, sf::Vector2f(450, 350 + 50 * (&pair - &*Constants::CHARACTERS.begin())));
    }

    // Set window properties
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // Initialize audio
    audioManager.generateSoundEffect("attack", 800, 0.1f, 50);
    audioManager.generateSoundEffect("special", 600, 0.3f, 70);
    audioManager.generateSoundEffect("hit", 200, 0.2f, 80);
    audioManager.generateSoundEffect("polarity_change", 1000, 0.5f, 60);
    audioManager.generateSoundEffect("victory", 523, 1.0f, 60);
    audioManager.generateSoundEffect("defeat", 220, 1.5f, 40);
    audioManager.generateSoundEffect("dialogue", 440, 0.5f, 30);

    // Generate sprites
    spriteGenerator.generateCharacterSprite("Koushi Shirota", Constants::SHIROTA_COLOR,
                                          sf::Color(42, 42, 74), sf::Color(100, 149, 237), "normal");
    spriteGenerator.generateCharacterSprite("Takane Takamine", Constants::TAKAMINE_COLOR,
                                          sf::Color(106, 42, 74), sf::Color(220, 20, 60), "tsundere");
    spriteGenerator.generateBackground("school_rooftop");
    spriteGenerator.generateBackground("classroom");

    loadCharacterData();
}

    // Setup main menu
    mainMenu.setTitle("SHIROTA: BLOOD POLARITY ULTIMATE", sf::Vector2f(200, 150));
    mainMenu.addItem("Story Mode", sf::Vector2f(450, 300));
    mainMenu.addItem("Arcade Mode", sf::Vector2f(450, 350));
    mainMenu.addItem("Survival Mode", sf::Vector2f(450, 400));
    mainMenu.addItem("Versus Mode", sf::Vector2f(450, 450));
    mainMenu.addItem("Training", sf::Vector2f(450, 500));
    mainMenu.addItem("Achievements", sf::Vector2f(450, 550));
    mainMenu.addItem("Settings", sf::Vector2f(450, 600));
    mainMenu.addItem("Exit", sf::Vector2f(450, 650));

    // Setup mode menu
    modeMenu.setTitle("SELECT MODE", sf::Vector2f(400, 200));
    modeMenu.addItem("Easy", sf::Vector2f(450, 350));
    modeMenu.addItem("Normal", sf::Vector2f(450, 400));
    modeMenu.addItem("Hard", sf::Vector2f(450, 450));
    modeMenu.addItem("Expert", sf::Vector2f(450, 500));

    // Setup character menu
    characterMenu.setTitle("SELECT CHARACTER", sf::Vector2f(350, 200));
    for (const auto& pair : Constants::CHARACTERS) {
        characterMenu.addItem(pair.first, sf::Vector2f(450, 350 + 50 * (&pair - &*Constants::CHARACTERS.begin())));
    }

    // Set window properties
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // Initialize audio
    audioManager.generateSoundEffect("attack", 800, 0.1f, 50);
    audioManager.generateSoundEffect("special", 600, 0.3f, 70);
    audioManager.generateSoundEffect("hit", 200, 0.2f, 80);
    audioManager.generateSoundEffect("polarity_change", 1000, 0.5f, 60);
    audioManager.generateSoundEffect("victory", 523, 1.0f, 60); // C5 note
    audioManager.generateSoundEffect("defeat", 220, 1.5f, 40); // A3 note

    // Version info
    versionText.setFont(font);
    versionText.setCharacterSize(14);
    versionText.setFillColor(sf::Color::White);
    versionText.setPosition(10, Constants::WINDOW_HEIGHT - 25);
    versionText.setString("Shirota: Blood Polarity Ultimate v2.0");

    loadCharacterData();
}

Game::~Game() {
    delete player;
    delete enemy;
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        gameTime += deltaTime;

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (gameState == GameState::MENU) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    mainMenu.moveUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    mainMenu.moveDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = mainMenu.getSelectedItem();
                    switch (selected) {
                        case 0: changeState(GameState::STORY_MODE); break;
                        case 1: changeState(GameState::ARCADE_MODE); break;
                        case 2: changeState(GameState::SURVIVAL_MODE); break;
                        case 3: changeState(GameState::VERSUS_MODE); break;
                        case 4: changeState(GameState::TRAINING_MODE); break;
                        case 5: changeState(GameState::ACHIEVEMENTS); break;
                        case 6: changeState(GameState::SETTINGS); break;
                        case 7: window.close(); break;
                    }
                }
            }
        } else if (gameState == GameState::CHARACTER_SELECT) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    characterMenu.moveUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    characterMenu.moveDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = characterMenu.getSelectedItem();
                    auto it = Constants::CHARACTERS.begin();
                    std::advance(it, selected);
                    selectedCharacter = it->first;
                    createCharacter(selectedCharacter, true);
                    changeState(GameState::LEVEL_SELECT);
                } else if (event.key.code == sf::Keyboard::Escape) {
                    changeState(previousState);
                }
            }
        } else if (gameState == GameState::PLAYING) {
            if (event.type == sf::Event::KeyPressed) {
                if (player) player->keys[event.key.code] = true;

                if (event.key.code == sf::Keyboard::Escape) {
                    changeState(GameState::PAUSED);
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (player) player->keys[event.key.code] = false;
            }
        } else if (gameState == GameState::PAUSED) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    changeState(GameState::PLAYING);
                } else if (event.key.code == sf::Keyboard::Q) {
                    changeState(GameState::MENU);
                }
            }
        } else if (gameState == GameState::GAME_OVER) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    wins++;
                    saveSystem.addWin();
                    handleAchievements();
                    changeState(previousState);
                }
            }
        } else if (gameState == GameState::ACHIEVEMENTS || gameState == GameState::SETTINGS) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                changeState(GameState::MENU);
            }
        }
    }
}

void Game::update(float deltaTime) {
    updateFPS(deltaTime);

    if (gameState == GameState::CUTSCENE) {
        if (storyManager) {
            storyManager->update(deltaTime);
            if (storyManager->isStoryComplete()) {
                changeState(GameState::GAME_OVER);
                audioManager.playSound("victory");
                return;
            }
        }
    } else if (gameState == GameState::PLAYING || gameState == GameState::STORY_MODE ||
               gameState == GameState::ARCADE_MODE || gameState == GameState::SURVIVAL_MODE ||
               gameState == GameState::VERSUS_MODE) {

        // Update story mode if active
        if (storyMode) {
            storyMode->update(deltaTime);
            if (storyMode->isComplete()) {
                changeState(GameState::GAME_OVER);
                audioManager.playSound("victory");
                return;
            }
        }

        // Update player input
        if (player) player->handleInput(&audioManager);

        // Update fighters
        if (enemy) enemy->updateAI(deltaTime, player.get(), &audioManager);
        if (player) player->update(deltaTime, enemy.get());
        if (enemy) enemy->update(deltaTime, player.get());

        // Check attack collisions
        if (player && enemy && checkAttackCollision(player.get(), enemy.get())) {
            int damage = player->attackType == "normal" ? 5 + static_cast<int>(player->comboCount * 1.5f) :
                         (player->attackType == "lightSpecial" || player->attackType == "darkSpecial") ? 25 : 5;
            enemy->takeDamage(damage, player.get());
            player->specialMeter = std::min(player->maxSpecial, player->specialMeter + 15);

            // Play attack sound
            if (player->attackType == "normal") {
                audioManager.playSound("attack");
            } else {
                audioManager.playSound("special");
            }
        }

        if (player && enemy && checkAttackCollision(enemy.get(), player.get())) {
            int damage = enemy->attackType == "normal" ? 5 : 25;
            player->takeDamage(damage, enemy.get());
            enemy->specialMeter = std::min(enemy->maxSpecial, enemy->specialMeter + 15);

            // Play attack sound
            if (enemy->attackType == "normal") {
                audioManager.playSound("attack");
            } else {
                audioManager.playSound("special");
            }
            audioManager.playSound("hit");
        }

        // Check win/lose
        if (player && player->getHealth() <= 0) {
            gameState = GameState::GAME_OVER;
            audioManager.playSound("defeat");
        } else if (enemy && enemy->getHealth() <= 0) {
            wins++;
            saveSystem.addWin();
            handleAchievements();
            if (storyMode) {
                storyMode->nextLevel();
            } else if (storyManager && storyManager->getCurrentLevel()) {
                // Story progression
                resetGame();
            } else {
                resetGame();
            }
            audioManager.playSound("victory");
        }
    }

    // Update particles
    particles.update(deltaTime);
}
        }

        // Update player input
        if (player) player->handleInput(&audioManager);

        // Update fighters
        if (enemy) enemy->updateAI(deltaTime, player.get(), &audioManager);
        if (player) player->update(deltaTime, enemy.get());
        if (enemy) enemy->update(deltaTime, player.get());

        // Check attack collisions
        if (player && enemy && checkAttackCollision(player.get(), enemy.get())) {
            int damage = player->attackType == "normal" ? 5 + static_cast<int>(player->comboCount * 1.5f) :
                         (player->attackType == "lightSpecial" || player->attackType == "darkSpecial") ? 25 : 5;
            enemy->takeDamage(damage, player.get());
            player->specialMeter = std::min(player->maxSpecial, player->specialMeter + 15);

            // Play attack sound
            if (player->attackType == "normal") {
                audioManager.playSound("attack");
            } else {
                audioManager.playSound("special");
            }
        }

        if (player && enemy && checkAttackCollision(enemy.get(), player.get())) {
            int damage = enemy->attackType == "normal" ? 5 : 25;
            player->takeDamage(damage, enemy.get());
            enemy->specialMeter = std::min(enemy->maxSpecial, enemy->specialMeter + 15);

            // Play attack sound
            if (enemy->attackType == "normal") {
                audioManager.playSound("attack");
            } else {
                audioManager.playSound("special");
            }
            audioManager.playSound("hit");
        }

        // Check win/lose
        if (player && player->getHealth() <= 0) {
            gameState = GameState::GAME_OVER;
            audioManager.playSound("defeat");
        } else if (enemy && enemy->getHealth() <= 0) {
            wins++;
            saveSystem.addWin();
            handleAchievements();
            if (storyMode) {
                storyMode->nextLevel();
            } else {
                resetGame();
            }
            audioManager.playSound("victory");
        }
    }

    // Update particles
    particles.update(deltaTime);
}

void Game::render() {
    window.clear();

    switch (gameState) {
        case GameState::MENU:
            drawMenu();
            break;
        case GameState::CHARACTER_SELECT:
            drawCharacterSelect();
            break;
        case GameState::LEVEL_SELECT:
            drawModeSelect();
            break;
        case GameState::ACHIEVEMENTS:
            drawAchievements();
            break;
        case GameState::SETTINGS:
            drawSettings();
            break;
        case GameState::PLAYING:
        case GameState::STORY_MODE:
        case GameState::ARCADE_MODE:
        case GameState::SURVIVAL_MODE:
        case GameState::VERSUS_MODE:
        case GameState::TRAINING_MODE:
            drawBackground();
            particles.draw(window);
            if (player) player->draw(window, font, particles);
            if (enemy) enemy->draw(window, font, particles);
            if (storyMode) storyMode->render(window, font);
            if (storyManager) storyManager->render(window);
            drawUI();
            break;
        case GameState::CUTSCENE:
            if (storyManager) storyManager->render(window);
            break;
        case GameState::PAUSED:
            drawBackground();
            particles.draw(window);
            if (player) player->draw(window, font, particles);
            if (enemy) enemy->draw(window, font, particles);
            drawUI();
            drawPauseMenu();
            break;
        case GameState::GAME_OVER:
            drawBackground();
            particles.draw(window);
            if (player) player->draw(window, font, particles);
            if (enemy) enemy->draw(window, font, particles);
            drawUI();
            drawGameOver();
            break;
    }

    window.draw(versionText);
    window.draw(fpsText);
    window.display();
}

void Game::resetGame() {
    player->health = player->maxHealth;
    enemy->health = enemy->maxHealth;
    player->position = sf::Vector2f(200, 500);
    enemy->position = sf::Vector2f(900, 500);
    player->velocity = sf::Vector2f(0, 0);
    enemy->velocity = sf::Vector2f(0, 0);
    player->polarity = Constants::Polarity::LIGHT;
    enemy->polarity = Constants::Polarity::DARK;
    player->specialMeter = 0;
    enemy->specialMeter = 0;
    player->comboCount = 0;
    enemy->comboCount = 0;
    gameState = GameState::PLAYING;
    particles.clear();
}

bool Game::checkAttackCollision(Fighter* attacker, Fighter* target) {
    if (!attacker->isAttacking || attacker->attackTimer <= 0) return false;

    float reach = 60.0f;
    if (attacker->attackType == "lightSpecial" || attacker->attackType == "darkSpecial") {
        reach = 100.0f;
    }

    float ax = attacker->getPosition().x + (attacker->facing == 1 ? attacker->getSize().x : -reach);
    float ay = attacker->getPosition().y;
    float aw = reach;
    float ah = attacker->getSize().y;

    // AABB collision
    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f targetSize = target->getSize();

    if (ax < targetPos.x + targetSize.x &&
        ax + aw > targetPos.x &&
        ay < targetPos.y + targetSize.y &&
        ay + ah > targetPos.y) {
        return true;
    }

    return false;
}

void Game::drawBackground() {
    // Sky gradient
    sf::VertexArray sky(sf::Quads, 4);
    sky[0].position = sf::Vector2f(0, 0);
    sky[1].position = sf::Vector2f(Constants::WINDOW_WIDTH, 0);
    sky[2].position = sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    sky[3].position = sf::Vector2f(0, Constants::WINDOW_HEIGHT);

    sky[0].color = sf::Color(10, 10, 26);
    sky[1].color = sf::Color(26, 5, 16);
    sky[2].color = sf::Color(15, 10, 26);
    sky[3].color = sf::Color(5, 5, 5);

    window.draw(sky);

    // Stars
    for (int i = 0; i < 50; i++) {
        float x = (i * 19 + gameTime * 0.1f) * 60 % Constants::WINDOW_WIDTH;
        float y = (i * 7) % (Constants::WINDOW_HEIGHT - 100);
        float size = (std::sin(gameTime * 0.2f + i) + 1) * 0.5f;

        sf::CircleShape star(size);
        star.setPosition(x, y);
        star.setFillColor(sf::Color(255, 255, 255, 127));
        window.draw(star);
    }

    // Moon
    sf::CircleShape moon(40);
    moon.setPosition(Constants::WINDOW_WIDTH - 140, 80);
    moon.setFillColor(sf::Color(255, 255, 255, 25));
    window.draw(moon);

    // Ground
    sf::RectangleShape ground(sf::Vector2f(Constants::WINDOW_WIDTH, 40));
    ground.setPosition(0, Constants::WINDOW_HEIGHT - 40);
    ground.setFillColor(sf::Color(42, 26, 26));
    window.draw(ground);

    // Platform
    sf::RectangleShape platform(sf::Vector2f(600, 10));
    platform.setPosition(300, Constants::WINDOW_HEIGHT - 140);
    platform.setFillColor(sf::Color(100, 50, 50, 63));
    window.draw(platform);
}

void Game::drawUI() {
    // Title
    sf::Text titleText("SHIROTA: BLOOD POLARITY", font, 36);
    titleText.setPosition(Constants::WINDOW_WIDTH/2 - titleText.getLocalBounds().width/2, 10);
    sf::Color titleColor = sf::Color(255, 68, 68);
    titleText.setFillColor(titleColor);
    window.draw(titleText);

    // Controls
    sf::Text controlsText("A/D: Move | W: Jump | SPACE: Attack | Q/E: Special | SHIFT: Change Polarity | V: Dash | S: Block | ESC: Pause", font, 14);
    controlsText.setPosition(10, Constants::WINDOW_HEIGHT - 30);
    controlsText.setFillColor(sf::Color(170, 170, 170));
    window.draw(controlsText);

    // Wins counter
    sf::Text winsText("Wins: " + std::to_string(wins), font, 18);
    winsText.setPosition(Constants::WINDOW_WIDTH - 100, 10);
    winsText.setFillColor(sf::Color::White);
    window.draw(winsText);
}

void Game::drawMenu() {
    // Background
    sf::RectangleShape bg(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
    bg.setFillColor(sf::Color(10, 10, 26));
    window.draw(bg);

    // Stars
    for (int i = 0; i < 100; i++) {
        sf::CircleShape star(1);
        star.setPosition(rand() % Constants::WINDOW_WIDTH, rand() % Constants::WINDOW_HEIGHT);
        star.setFillColor(sf::Color(255, 255, 255, 100));
        window.draw(star);
    }

    mainMenu.draw(window);
}

void Game::drawPauseMenu() {
    sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    sf::Text pauseText("PAUSED", font, 56);
    pauseText.setPosition(Constants::WINDOW_WIDTH/2 - pauseText.getLocalBounds().width/2,
                         Constants::WINDOW_HEIGHT/2 - 40);
    pauseText.setFillColor(sf::Color::White);
    window.draw(pauseText);

    sf::Text resumeText("Press ESC to resume", font, 24);
    resumeText.setPosition(Constants::WINDOW_WIDTH/2 - resumeText.getLocalBounds().width/2,
                          Constants::WINDOW_HEIGHT/2 + 40);
    resumeText.setFillColor(sf::Color::White);
    window.draw(resumeText);
}

void Game::drawGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    sf::Text gameOverText("VICTORY!", font, 56);
    gameOverText.setPosition(Constants::WINDOW_WIDTH/2 - gameOverText.getLocalBounds().width/2,
                            Constants::WINDOW_HEIGHT/2 - 40);
    gameOverText.setFillColor(sf::Color::White);
    window.draw(gameOverText);

    sf::Text restartText("Press SPACE to continue", font, 24);
    restartText.setPosition(Constants::WINDOW_WIDTH/2 - restartText.getLocalBounds().width/2,
                           Constants::WINDOW_HEIGHT/2 + 40);
    restartText.setFillColor(sf::Color::White);
    window.draw(restartText);
}