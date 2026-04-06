#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Fighter.h"
#include "Particle.h"
#include "EffectManager.h"

class Level {
private:
    std::string name;
    std::string description;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<Fighter*> enemies;
    std::string backgroundMusic;
    int difficulty;
    sf::Vector2f playerStartPos;
    std::vector<sf::Vector2f> enemyStartPos;
    EffectManager effectManager;
    bool levelComplete;
    float timeLimit;
    sf::Clock levelTimer;

    // Environment objects
    std::vector<sf::RectangleShape> platforms;
    std::vector<sf::CircleShape> interactiveObjects;

public:
    Level(const Constants::LevelData& data);
    ~Level();

    void update(float deltaTime);
    void render(sf::RenderWindow& window, sf::Font& font);
    bool isComplete() const;
    const std::vector<Fighter*>& getEnemies() const;
    sf::Vector2f getPlayerStartPos() const;
    bool checkCollision(const sf::Vector2f& pos, const sf::Vector2f& size) const;

    // Level-specific effects
    void triggerSpecialEffect(const std::string& effectName, const sf::Vector2f& position);
    void updateEnvironment(float deltaTime);
};

#endif // LEVEL_H