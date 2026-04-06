#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include <SFML/Graphics.hpp>
#include "Particle.h"

class EffectManager {
private:
    ParticleSystem particles;
    std::vector<sf::Sprite> effects;
    sf::Clock effectClock;

public:
    EffectManager();
    ~EffectManager();

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    // Effect creation methods
    void createExplosion(const sf::Vector2f& position);
    void createLightning(const sf::Vector2f& position);
    void createFire(const sf::Vector2f& position);
    void createAura(const sf::Vector2f& position, sf::Color color);
    void createShockwave(const sf::Vector2f& position);

    // Get particles for external use
    ParticleSystem& getParticles();
};

#endif // EFFECT_MANAGER_H