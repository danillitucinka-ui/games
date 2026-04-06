#include "AudioManager.h"
#include <cmath>
#include <iostream>

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    stopBackgroundMusic();
}

void AudioManager::loadSound(const std::string& name, const std::string& filename) {
    if (soundBuffers[name].loadFromFile(filename)) {
        sounds[name].setBuffer(soundBuffers[name]);
    } else {
        std::cout << "Error loading sound: " << filename << std::endl;
    }
}

void AudioManager::playSound(const std::string& name) {
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play();
    }
}

void AudioManager::playBackgroundMusic(const std::string& filename) {
    if (backgroundMusic.openFromFile(filename)) {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    } else {
        std::cout << "Error loading background music: " << filename << std::endl;
    }
}

void AudioManager::stopBackgroundMusic() {
    backgroundMusic.stop();
}

void AudioManager::generateSoundEffect(const std::string& name, float frequency, float duration, float volume) {
    const unsigned int sampleRate = 44100;
    const unsigned int sampleCount = static_cast<unsigned int>(sampleRate * duration);
    std::vector<sf::Int16> samples(sampleCount);

    // Generate sine wave for attack sound
    for (unsigned int i = 0; i < sampleCount; ++i) {
        float time = static_cast<float>(i) / sampleRate;
        float envelope = 1.0f - (time / duration); // Fade out
        samples[i] = static_cast<sf::Int16>(volume * envelope * std::sin(2 * M_PI * frequency * time));
    }

    soundBuffers[name].loadFromSamples(&samples[0], sampleCount, 1, sampleRate);
    sounds[name].setBuffer(soundBuffers[name]);
}
