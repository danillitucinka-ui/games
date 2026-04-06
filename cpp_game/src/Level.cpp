#include "Level.h"
#include <iostream>

Level::Level(const Constants::LevelData& data)
    : name(data.name), description(data.description), backgroundMusic(data.musicPath),
      difficulty(data.difficulty), levelComplete(false), timeLimit(300.0f) {

    // Load background (placeholder - create programmatically)
    backgroundTexture.create(1200, 700);
    backgroundSprite.setTexture(backgroundTexture);

    // Create background based on level
    if (name == "School Rooftop") {
        // Create school rooftop background
        sf::Image bgImage;
        bgImage.create(1200, 700, sf::Color(135, 206, 235)); // Sky blue

        // Add buildings, sky, etc.
        for (int x = 0; x < 1200; x++) {
            for (int y = 0; y < 700; y++) {
                if (y > 600) bgImage.setPixel(x, y, sf::Color(100, 100, 100)); // Ground
                else if (y > 500 && x > 200 && x < 1000) bgImage.setPixel(x, y, sf::Color(200, 200, 200)); // Buildings
            }
        }
        backgroundTexture.update(bgImage);
    } else if (name == "Classroom Chaos") {
        sf::Image bgImage;
        bgImage.create(1200, 700, sf::Color(240, 240, 220)); // Cream color

        // Add desks, chairs, blackboard
        for (int x = 0; x < 1200; x++) {
            for (int y = 0; y < 700; y++) {
                if (y > 650) bgImage.setPixel(x, y, sf::Color(150, 100, 50)); // Floor
                else if (y > 400 && y < 500) bgImage.setPixel(x, y, sf::Color(50, 50, 150)); // Blackboard
                else if ((x % 200 < 50 || y % 100 < 30) && y > 500) bgImage.setPixel(x, y, sf::Color(100, 50, 0)); // Desks
            }
        }
        backgroundTexture.update(bgImage);
    }
    // Add more level backgrounds...

    playerStartPos = sf::Vector2f(100, 500);

    // Create platforms
    platforms.emplace_back(sf::Vector2f(600, 10));
    platforms.back().setPosition(300, Constants::WINDOW_HEIGHT - 140);
    platforms.back().setFillColor(sf::Color(100, 50, 50, 127));

    platforms.emplace_back(sf::Vector2f(200, 10));
    platforms.back().setPosition(50, Constants::WINDOW_HEIGHT - 300);
    platforms.back().setFillColor(sf::Color(100, 50, 50, 127));

    platforms.emplace_back(sf::Vector2f(200, 10));
    platforms.back().setPosition(950, Constants::WINDOW_HEIGHT - 300);
    platforms.back().setFillColor(sf::Color(100, 50, 50, 127));
}

Level::~Level() {
    for (auto enemy : enemies) {
        delete enemy;
    }
}

void Level::update(float deltaTime) {
    // Update enemies
    for (auto enemy : enemies) {
        if (enemy) enemy->update(deltaTime, nullptr); // Simplified
    }

    // Update environment
    updateEnvironment(deltaTime);

    // Check time limit
    if (levelTimer.getElapsedTime().asSeconds() > timeLimit) {
        levelComplete = true; // Time up
    }

    // Check if all enemies defeated
    bool allDefeated = true;
    for (auto enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            allDefeated = false;
            break;
        }
    }
    if (allDefeated) {
        levelComplete = true;
    }
}

void Level::render(sf::RenderWindow& window, sf::Font& font) {
    window.draw(backgroundSprite);

    // Draw platforms
    for (const auto& platform : platforms) {
        window.draw(platform);
    }

    // Draw interactive objects
    for (const auto& obj : interactiveObjects) {
        window.draw(obj);
    }

    // Draw enemies
    for (auto enemy : enemies) {
        if (enemy) enemy->draw(window, font, effectManager.getParticles());
    }

    // Draw effects
    effectManager.draw(window);

    // Draw level info
    sf::Text levelText(name, font, 24);
    levelText.setPosition(10, 10);
    levelText.setFillColor(sf::Color::White);
    window.draw(levelText);

    // Draw timer
    float remainingTime = timeLimit - levelTimer.getElapsedTime().asSeconds();
    sf::Text timerText("Time: " + std::to_string(static_cast<int>(remainingTime)), font, 18);
    timerText.setPosition(Constants::WINDOW_WIDTH - 150, 10);
    timerText.setFillColor(remainingTime < 30 ? sf::Color::Red : sf::Color::White);
    window.draw(timerText);
}

bool Level::isComplete() const {
    return levelComplete;
}

const std::vector<Fighter*>& Level::getEnemies() const {
    return enemies;
}

sf::Vector2f Level::getPlayerStartPos() const {
    return playerStartPos;
}

bool Level::checkCollision(const sf::Vector2f& pos, const sf::Vector2f& size) const {
    // Check platform collisions
    for (const auto& platform : platforms) {
        sf::FloatRect platformRect = platform.getGlobalBounds();
        sf::FloatRect entityRect(pos.x, pos.y, size.x, size.y);

        if (entityRect.intersects(platformRect)) {
            return true;
        }
    }
    return false;
}

void Level::triggerSpecialEffect(const std::string& effectName, const sf::Vector2f& position) {
    if (effectName == "explosion") {
        effectManager.createExplosion(position);
    } else if (effectName == "lightning") {
        effectManager.createLightning(position);
    } else if (effectName == "fire") {
        effectManager.createFire(position);
    }
}

void Level::updateEnvironment(float deltaTime) {
    // Animate interactive objects, weather effects, etc.
    static float animationTime = 0;
    animationTime += deltaTime;

    // Example: floating particles in some levels
    if (name == "School Garden") {
        // Add cherry blossom petals
        if (rand() % 100 < 2) {
            sf::CircleShape petal(2);
            petal.setPosition(rand() % Constants::WINDOW_WIDTH, 0);
            petal.setFillColor(sf::Color(255, 192, 203, 150));
            interactiveObjects.push_back(petal);
        }

        // Remove old petals
        for (auto it = interactiveObjects.begin(); it != interactiveObjects.end();) {
            it->move(0, 20 * deltaTime);
            if (it->getPosition().y > Constants::WINDOW_HEIGHT) {
                it = interactiveObjects.erase(it);
            } else {
                ++it;
            }
        }
    }
}