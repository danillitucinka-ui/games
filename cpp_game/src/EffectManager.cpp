#include "EffectManager.h"
#include <cmath>

EffectManager::EffectManager() {}

EffectManager::~EffectManager() {}

void EffectManager::update(float deltaTime) {
    particles.update(deltaTime);

    // Update effect sprites (fade out, etc.)
    for (auto it = effects.begin(); it != effects.end();) {
        sf::Color color = it->getColor();
        color.a -= static_cast<sf::Uint8>(200 * deltaTime);
        it->setColor(color);

        if (color.a <= 10) {
            it = effects.erase(it);
        } else {
            ++it;
        }
    }
}

void EffectManager::draw(sf::RenderWindow& window) {
    particles.draw(window);

    for (auto& effect : effects) {
        window.draw(effect);
    }
}

void EffectManager::createExplosion(const sf::Vector2f& position) {
    // Create explosion particles
    for (int i = 0; i < 50; ++i) {
        float angle = (i / 50.0f) * 2 * 3.14159f;
        float speed = 100 + rand() % 200;
        sf::Color color = sf::Color(255, 100 + rand() % 100, 0);
        particles.addParticle(position.x, position.y, color,
                           std::cos(angle) * speed, std::sin(angle) * speed,
                           5 + rand() % 10, 1.0f + rand() % 2);
    }

    // Create explosion sprite
    sf::CircleShape explosion(50);
    explosion.setPosition(position.x - 50, position.y - 50);
    explosion.setFillColor(sf::Color(255, 150, 0, 150));
    effects.push_back(explosion);
}

void EffectManager::createLightning(const sf::Vector2f& position) {
    // Create lightning particles
    for (int i = 0; i < 30; ++i) {
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 50 + rand() % 100;
        particles.addParticle(position.x, position.y, sf::Color::Cyan,
                           std::cos(angle) * speed, std::sin(angle) * speed,
                           2 + rand() % 3, 0.5f + rand() % 1);
    }

    // Create lightning bolt sprite (simplified)
    sf::RectangleShape bolt(sf::Vector2f(5, 100));
    bolt.setPosition(position);
    bolt.setFillColor(sf::Color::Cyan);
    bolt.setRotation(rand() % 360);
    effects.push_back(bolt);
}

void EffectManager::createFire(const sf::Vector2f& position) {
    // Create fire particles
    for (int i = 0; i < 25; ++i) {
        float angle = -3.14159f/2 + (rand() % 100 - 50) * 3.14159f / 180.0f;
        float speed = 20 + rand() % 40;
        sf::Color color = sf::Color(255, 100 + rand() % 100, 0);
        particles.addParticle(position.x, position.y, color,
                           std::cos(angle) * speed, std::sin(angle) * speed - 10,
                           3 + rand() % 4, 1.5f + rand() % 1);
    }
}

void EffectManager::createAura(const sf::Vector2f& position, sf::Color color) {
    // Create aura particles
    for (int i = 0; i < 20; ++i) {
        float angle = (i / 20.0f) * 2 * 3.14159f;
        float radius = 30 + rand() % 20;
        float x = position.x + std::cos(angle) * radius;
        float y = position.y + std::sin(angle) * radius;
        particles.addParticle(x, y, color, 0, 0, 4, 2.0f);
    }
}

void EffectManager::createShockwave(const sf::Vector2f& position) {
    // Create shockwave effect
    sf::CircleShape wave(10);
    wave.setPosition(position.x - 10, position.y - 10);
    wave.setFillColor(sf::Color::Transparent);
    wave.setOutlineColor(sf::Color::White);
    wave.setOutlineThickness(3);
    effects.push_back(wave);
}

ParticleSystem& EffectManager::getParticles() {
    return particles;
}