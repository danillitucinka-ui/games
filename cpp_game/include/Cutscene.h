#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Cutscene {
private:
    std::vector<std::string> dialogueLines;
    std::vector<sf::Sprite> characters;
    std::vector<sf::Text> textElements;
    sf::Font font;
    int currentLine;
    float textTimer;
    bool showingText;
    sf::Clock sceneTimer;

public:
    Cutscene(sf::Font& f);
    ~Cutscene();

    void addDialogue(const std::string& speaker, const std::string& text);
    void addCharacter(const sf::Texture& texture, sf::Vector2f position);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isFinished() const;
    void skip();

    // Scene transitions
    void fadeIn(float deltaTime);
    void fadeOut(float deltaTime);
    void panCamera(sf::Vector2f target, float speed, float deltaTime);
};

#endif // CUTSCENE_H