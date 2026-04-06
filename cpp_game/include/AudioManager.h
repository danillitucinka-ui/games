#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

class AudioManager {
private:
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    sf::Music backgroundMusic;

public:
    AudioManager();
    ~AudioManager();

    void loadSound(const std::string& name, const std::string& filename);
    void playSound(const std::string& name);
    void playBackgroundMusic(const std::string& filename);
    void stopBackgroundMusic();
    void generateSoundEffect(const std::string& name, float frequency, float duration, float volume = 100.0f);
};

#endif // AUDIO_MANAGER_H

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    stopBackgroundMusic();
}

void AudioManager::loadSound(const std::string& name, const std::string& filename) {
    if (soundBuffers[name].loadFromFile(filename)) {
        sounds[name].setBuffer(soundBuffers[name]);
    } else {
        // std::cout << "Error loading sound: " << filename << std::endl;
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
        // std::cout << "Error loading background music: " << filename << std::endl;
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
        samples[i] = static_cast<sf::Int16>(volume * envelope * std::sin(2 * 3.14159f * frequency * time));
    }

    soundBuffers[name].loadFromSamples(&samples[0], sampleCount, 1, sampleRate);
    sounds[name].setBuffer(soundBuffers[name]);
}