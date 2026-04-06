#include "Animation.h"

Animation::Animation() : totalDuration(0), currentTime(0), loop(false), finished(false) {}

Animation::~Animation() {}

void Animation::addFrame(const sf::Texture& texture, float duration) {
    frames.push_back(texture);
    frameDurations.push_back(duration);
    totalDuration += duration;
}

void Animation::update(float deltaTime) {
    if (finished && !loop) return;

    currentTime += deltaTime;

    if (currentTime >= totalDuration) {
        if (loop) {
            currentTime = std::fmod(currentTime, totalDuration);
            finished = false;
        } else {
            finished = true;
            currentTime = totalDuration;
        }
    }
}

void Animation::render(sf::RenderWindow& window, const sf::Vector2f& position) {
    if (frames.empty()) return;

    // Find current frame
    float time = 0;
    size_t currentFrame = 0;
    for (size_t i = 0; i < frameDurations.size(); ++i) {
        time += frameDurations[i];
        if (currentTime < time) {
            currentFrame = i;
            break;
        }
    }

    sprite.setTexture(frames[currentFrame]);
    sprite.setPosition(position);
    window.draw(sprite);
}

void Animation::setLoop(bool looping) {
    loop = looping;
}

bool Animation::isFinished() const {
    return finished;
}

void Animation::reset() {
    currentTime = 0;
    finished = false;
}

void Animation::setScale(float scale) {
    sprite.setScale(scale, scale);
}

void Animation::setColor(const sf::Color& color) {
    sprite.setColor(color);
}

void AnimationManager::loadAnimation(const std::string& name, const std::vector<std::string>& framePaths) {
    Animation anim;
    for (const auto& path : framePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            anim.addFrame(texture, 0.1f); // 0.1s per frame
        }
    }
    animations[name] = anim;
}

Animation& AnimationManager::getAnimation(const std::string& name) {
    return animations[name];
}

bool AnimationManager::hasAnimation(const std::string& name) const {
    return animations.find(name) != animations.end();
}