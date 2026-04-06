#ifndef SPRITE_GENERATOR_H
#define SPRITE_GENERATOR_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class SpriteGenerator {
private:
    std::map<std::string, sf::Texture> textures;

public:
    SpriteGenerator();
    ~SpriteGenerator();

    // Generate character sprites
    sf::Texture generateCharacterSprite(const std::string& characterName,
                                       sf::Color bodyColor,
                                       sf::Color hairColor,
                                       sf::Color eyeColor,
                                       const std::string& expression = "normal");

    // Generate effect sprites
    sf::Texture generateAuraSprite(sf::Color color, float radius);
    sf::Texture generateParticleSprite(sf::Color color, float size);

    // Generate background sprites
    sf::Texture generateBackground(const std::string& location);

    // Generate UI elements
    sf::Texture generateHealthBar(float width, float height);
    sf::Texture generateButton(const std::string& text, sf::Font& font);

    // Get generated texture
    sf::Texture& getTexture(const std::string& key);

    // Save texture to file (for debugging)
    void saveTexture(const sf::Texture& texture, const std::string& filename);
};

#endif // SPRITE_GENERATOR_H