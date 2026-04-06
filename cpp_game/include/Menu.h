#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#ifndef MENU_H
#define MENU_H

class Menu {
private:
    sf::Font font;
    std::vector<sf::Text> menuItems;
    int selectedItem;
    sf::Text title;

public:
    Menu(sf::Font& f);
    void addItem(const std::string& text, sf::Vector2f position);
    void setTitle(const std::string& text, sf::Vector2f position);
    void moveUp();
    void moveDown();
    int getSelectedItem() const;
    void draw(sf::RenderWindow& window);
};

#endif // MENU_H

Menu::Menu(sf::Font& f) : font(f), selectedItem(0) {}

void Menu::addItem(const std::string& text, sf::Vector2f position) {
    sf::Text item(text, font, 24);
    item.setPosition(position);
    item.setFillColor(sf::Color::White);
    menuItems.push_back(item);
}

void Menu::setTitle(const std::string& text, sf::Vector2f position) {
    title.setString(text);
    title.setFont(font);
    title.setCharacterSize(48);
    title.setPosition(position);
    title.setFillColor(sf::Color(255, 68, 68));
}

void Menu::moveUp() {
    if (selectedItem > 0) selectedItem--;
}

void Menu::moveDown() {
    if (selectedItem < static_cast<int>(menuItems.size()) - 1) selectedItem++;
}

int Menu::getSelectedItem() const {
    return selectedItem;
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(title);
    for (size_t i = 0; i < menuItems.size(); ++i) {
        if (static_cast<int>(i) == selectedItem) {
            menuItems[i].setFillColor(sf::Color::Yellow);
        } else {
            menuItems[i].setFillColor(sf::Color::White);
        }
        window.draw(menuItems[i]);
    }
}