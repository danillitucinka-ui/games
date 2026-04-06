#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation {
private:
    std::vector<sf::Texture> frames;
    std::vector<float> frameDurations;
    sf::Sprite sprite;
    float totalDuration;
    float currentTime;
    bool loop;
    bool finished;

public:
    Animation();
    ~Animation();

    void addFrame(const sf::Texture& texture, float duration);
    void update(float deltaTime);
    void render(sf::RenderWindow& window, const sf::Vector2f& position);
    void setLoop(bool looping);
    bool isFinished() const;
    void reset();
    void setScale(float scale);
    void setColor(const sf::Color& color);
};

class AnimationManager {
private:
    std::map<std::string, Animation> animations;

public:
    void loadAnimation(const std::string& name, const std::vector<std::string>& framePaths);
    Animation& getAnimation(const std::string& name);
    bool hasAnimation(const std::string& name) const;
};

#endif // ANIMATION_H