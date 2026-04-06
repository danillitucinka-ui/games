#include "StoryMode.h"
#include <iostream>

StoryMode::StoryMode() : currentLevel(0), showingDialogue(false) {
    initialize();
}

StoryMode::~StoryMode() {}

void StoryMode::initialize() {
    for (const auto& levelData : Constants::LEVELS) {
        levels.emplace_back(levelData);
    }
    saveSystem.loadProgress();
    currentLevel = saveSystem.getCurrentLevel();
}

void StoryMode::update(float deltaTime) {
    if (showingDialogue) {
        if (dialogueTimer.getElapsedTime().asSeconds() > 3.0f || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            showingDialogue = false;
            dialogueTimer.restart();
        }
    } else {
        getCurrentLevel().update(deltaTime);
        handleStoryEvents();
    }
}

void StoryMode::render(sf::RenderWindow& window, sf::Font& font) {
    if (showingDialogue) {
        // Draw dialogue box
        sf::RectangleShape dialogueBox(sf::Vector2f(Constants::WINDOW_WIDTH - 100, 150));
        dialogueBox.setPosition(50, Constants::WINDOW_HEIGHT - 200);
        dialogueBox.setFillColor(sf::Color(0, 0, 0, 200));
        dialogueBox.setOutlineColor(sf::Color::White);
        dialogueBox.setOutlineThickness(2);
        window.draw(dialogueBox);

        sf::Text dialogueText(currentDialogue, font, 18);
        dialogueText.setPosition(70, Constants::WINDOW_HEIGHT - 180);
        dialogueText.setFillColor(sf::Color::White);
        window.draw(dialogueText);

        sf::Text skipText("Press SPACE to skip", font, 14);
        skipText.setPosition(Constants::WINDOW_WIDTH - 200, Constants::WINDOW_HEIGHT - 50);
        skipText.setFillColor(sf::Color::Yellow);
        window.draw(skipText);
    } else {
        getCurrentLevel().render(window, font);
    }
}

bool StoryMode::isComplete() const {
    return currentLevel >= static_cast<int>(levels.size());
}

Level& StoryMode::getCurrentLevel() {
    return levels[currentLevel];
}

void StoryMode::nextLevel() {
    if (currentLevel < static_cast<int>(levels.size()) - 1) {
        currentLevel++;
        saveSystem.setCurrentLevel(currentLevel);
        saveSystem.saveProgress();
    }
}

void StoryMode::showDialogue(const std::string& text) {
    currentDialogue = text;
    showingDialogue = true;
    dialogueTimer.restart();
}

bool StoryMode::isShowingDialogue() const {
    return showingDialogue;
}

void StoryMode::skipDialogue() {
    showingDialogue = false;
}

void StoryMode::handleStoryEvents() {
    // Story dialogue and events based on current level and progress
    static std::map<int, std::vector<std::string>> storyDialogues = {
        {0, {
            "Takamine: Why are you always staring at me, Shirota?",
            "Shirota: I... I was just...",
            "Takamine: Hmph! Don't think you can get close to me that easily!"
        }},
        {1, {
            "Miki: Hey Shirota! Want to join our study group?",
            "Shirota: Sure, but I have something else on my mind...",
            "Seiya: The air feels tense. Something's about to happen."
        }},
        {2, {
            "Kiriko: Beautiful flowers, aren't they? But beauty can be deceiving...",
            "Shirota: What do you mean?",
            "Kiriko: Some flowers have thorns that can hurt you badly."
        }},
        {3, {
            "Takamine: What are you doing out here so late?",
            "Shirota: I could ask you the same thing.",
            "Takamine: None of your business! Just stay out of my way!"
        }},
        {4, {
            "Seiya: This place gives me the creeps. Let's be careful.",
            "Kiriko: The shadows hide many secrets...",
            "Shirota: I feel like we're being watched."
        }},
        {5, {
            "Takamine: So this is where it ends, Shirota.",
            "Shirota: Takamine... I never wanted it to come to this.",
            "Takamine: Neither did I... but now we have no choice."
        }}
    };

    if (storyDialogues.count(currentLevel)) {
        static int dialogueIndex = 0;
        const auto& dialogues = storyDialogues[currentLevel];
        if (dialogueIndex < static_cast<int>(dialogues.size())) {
            // Show dialogue at specific points in the level
            // For simplicity, show all dialogues at the start
            if (!showingDialogue && dialogueIndex == 0) {
                showDialogue(dialogues[dialogueIndex]);
                dialogueIndex++;
            }
        }
    }
}

std::string StoryMode::getCurrentStoryText() const {
    return currentDialogue;
}