#include "Cutscene.h"

Cutscene::Cutscene(sf::Font& f) : font(f), currentLine(0), textTimer(0), showingText(false) {}

Cutscene::~Cutscene() {}

void Cutscene::addDialogue(const std::string& speaker, const std::string& text) {
    dialogueLines.push_back(speaker + ": " + text);
}

void Cutscene::addCharacter(const sf::Texture& texture, sf::Vector2f position) {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(position);
    characters.push_back(sprite);
}

void Cutscene::update(float deltaTime) {
    sceneTimer.restart();

    if (currentLine < static_cast<int>(dialogueLines.size())) {
        textTimer += deltaTime;

        if (!showingText && textTimer > 1.0f) { // Delay before showing text
            showingText = true;
            textTimer = 0;

            sf::Text text(dialogueLines[currentLine], font, 24);
            text.setPosition(50, 500);
            text.setFillColor(sf::Color::White);
            textElements.push_back(text);
        }

        if (showingText && textTimer > 3.0f) { // Text display duration
            showingText = false;
            textTimer = 0;
            currentLine++;
            textElements.clear();
        }
    }
}

void Cutscene::render(sf::RenderWindow& window) {
    // Draw background
    sf::RectangleShape bg(sf::Vector2f(1200, 700));
    bg.setFillColor(sf::Color(20, 20, 40));
    window.draw(bg);

    // Draw characters
    for (auto& sprite : characters) {
        window.draw(sprite);
    }

    // Draw dialogue box
    if (showingText) {
        sf::RectangleShape dialogueBox(sf::Vector2f(1100, 150));
        dialogueBox.setPosition(50, 520);
        dialogueBox.setFillColor(sf::Color(0, 0, 0, 200));
        dialogueBox.setOutlineColor(sf::Color::White);
        dialogueBox.setOutlineThickness(2);
        window.draw(dialogueBox);

        // Draw text
        for (auto& text : textElements) {
            window.draw(text);
        }

        // Draw continue indicator
        sf::Text continueText("Press SPACE to continue", font, 16);
        continueText.setPosition(1000, 650);
        continueText.setFillColor(sf::Color::Yellow);
        window.draw(continueText);
    }
}

bool Cutscene::isFinished() const {
    return currentLine >= static_cast<int>(dialogueLines.size());
}

void Cutscene::skip() {
    currentLine = dialogueLines.size();
}