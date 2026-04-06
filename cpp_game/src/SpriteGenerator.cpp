#include "SpriteGenerator.h"
#include <iostream>

SpriteGenerator::SpriteGenerator() {}

SpriteGenerator::~SpriteGenerator() {}

sf::Texture SpriteGenerator::generateCharacterSprite(const std::string& characterName,
                                                   sf::Color bodyColor,
                                                   sf::Color hairColor,
                                                   sf::Color eyeColor,
                                                   const std::string& expression) {
    sf::Image image;
    image.create(64, 96, sf::Color::Transparent);

    // Body (rectangle)
    for (int x = 16; x < 48; x++) {
        for (int y = 32; y < 80; y++) {
            image.setPixel(x, y, bodyColor);
        }
    }

    // Head (circle approximation)
    for (int x = 20; x < 44; x++) {
        for (int y = 8; y < 32; y++) {
            float dx = x - 32;
            float dy = y - 20;
            if (dx*dx + dy*dy <= 144) { // radius 12
                image.setPixel(x, y, sf::Color(255, 218, 164)); // Skin color
            }
        }
    }

    // Hair
    for (int x = 16; x < 48; x++) {
        for (int y = 4; y < 16; y++) {
            if ((x-32)*(x-32) + (y-10)*(y-10) <= 100) {
                image.setPixel(x, y, hairColor);
            }
        }
    }

    // Eyes
    int eyeY = 16;
    // Left eye
    for (int x = 24; x < 30; x++) {
        for (int y = eyeY; y < eyeY + 4; y++) {
            image.setPixel(x, y, sf::Color::White);
        }
    }
    image.setPixel(26, eyeY + 2, eyeColor); // Pupil

    // Right eye
    for (int x = 34; x < 40; x++) {
        for (int y = eyeY; y < eyeY + 4; y++) {
            image.setPixel(x, y, sf::Color::White);
        }
    }
    image.setPixel(36, eyeY + 2, eyeColor); // Pupil

    // Mouth (simple line)
    int mouthY = 24;
    for (int x = 28; x < 36; x++) {
        if (expression == "smile") {
            if (x >= 30 && x <= 34) image.setPixel(x, mouthY, sf::Color::Black);
        } else {
            image.setPixel(x, mouthY, sf::Color::Black);
        }
    }

    // Special features for characters
    if (characterName == "Takane Takamine") {
        // Add tsundere expression - slight blush
        for (int x = 20; x < 44; x++) {
            for (int y = 8; y < 20; y++) {
                sf::Color pixel = image.getPixel(x, y);
                if (pixel != sf::Color::Transparent) {
                    pixel.r = std::min(255, (int)pixel.r + 30);
                    image.setPixel(x, y, pixel);
                }
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    textures[characterName + "_" + expression] = texture;
    return texture;
}

sf::Texture SpriteGenerator::generateAuraSprite(sf::Color color, float radius) {
    sf::Image image;
    int size = static_cast<int>(radius * 2);
    image.create(size, size, sf::Color::Transparent);

    int centerX = size / 2;
    int centerY = size / 2;

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = std::sqrt(dx*dx + dy*dy);

            if (distance <= radius) {
                float alpha = (1.0f - distance / radius) * 128; // Fade out
                sf::Color pixelColor = color;
                pixelColor.a = static_cast<sf::Uint8>(alpha);
                image.setPixel(x, y, pixelColor);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    textures["aura_" + std::to_string(color.toInteger())] = texture;
    return texture;
}

sf::Texture SpriteGenerator::generateParticleSprite(sf::Color color, float size) {
    sf::Image image;
    int imgSize = static_cast<int>(size);
    image.create(imgSize, imgSize, sf::Color::Transparent);

    int center = imgSize / 2;
    for (int x = 0; x < imgSize; x++) {
        for (int y = 0; y < imgSize; y++) {
            float dx = x - center;
            float dy = y - center;
            if (dx*dx + dy*dy <= size*size/4) {
                image.setPixel(x, y, color);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    textures["particle_" + std::to_string(color.toInteger())] = texture;
    return texture;
}

sf::Texture SpriteGenerator::generateBackground(const std::string& location) {
    sf::Image image;
    image.create(1200, 700, sf::Color::Transparent);

    if (location == "school_rooftop") {
        // Sky gradient
        for (int y = 0; y < 400; y++) {
            sf::Color skyColor(135 - y/4, 206 - y/2, 235 - y);
            for (int x = 0; x < 1200; x++) {
                image.setPixel(x, y, skyColor);
            }
        }

        // Buildings
        for (int x = 200; x < 1000; x++) {
            for (int y = 300; y < 500; y++) {
                image.setPixel(x, y, sf::Color(150, 150, 150));
            }
        }

        // Rooftop
        for (int x = 0; x < 1200; x++) {
            for (int y = 500; y < 700; y++) {
                image.setPixel(x, y, sf::Color(100, 100, 100));
            }
        }

    } else if (location == "classroom") {
        // Walls
        for (int x = 0; x < 1200; x++) {
            for (int y = 0; y < 700; y++) {
                image.setPixel(x, y, sf::Color(240, 240, 220));
            }
        }

        // Blackboard
        for (int x = 200; x < 1000; x++) {
            for (int y = 100; y < 300; y++) {
                image.setPixel(x, y, sf::Color(50, 50, 50));
            }
        }

        // Desks
        for (int desk = 0; desk < 6; desk++) {
            int startX = 100 + desk * 150;
            for (int x = startX; x < startX + 80; x++) {
                for (int y = 400; y < 500; y++) {
                    image.setPixel(x, y, sf::Color(139, 69, 19));
                }
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    textures["bg_" + location] = texture;
    return texture;
}

sf::Texture SpriteGenerator::generateHealthBar(float width, float height) {
    sf::Image image;
    image.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height), sf::Color::Transparent);

    // Background
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            if (x < 2 || x >= width-2 || y < 2 || y >= height-2) {
                image.setPixel(x, y, sf::Color::Black);
            } else {
                image.setPixel(x, y, sf::Color(64, 64, 64));
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    textures["health_bar"] = texture;
    return texture;
}

sf::Texture& SpriteGenerator::getTexture(const std::string& key) {
    return textures[key];
}

void SpriteGenerator::saveTexture(const sf::Texture& texture, const std::string& filename) {
    sf::Image image = texture.copyToImage();
    image.saveToFile(filename);
}