#ifndef MENU_HPP
#define MENU_HPP 

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class Menu {
public:
    Menu(const sf::Font& font, const sf::Vector2u& windowSize) {
        // Set up the title text
        title.setFont(font);
        title.setString("Tetris");
        title.setCharacterSize(64);
        title.setFillColor(sf::Color::White);
        // Center title
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.0f,
                        titleBounds.top + titleBounds.height / 2.0f);
        title.setPosition(windowSize.x / 2.0f, windowSize.y / 4.0f);
        
        // Create menu options
        sf::Text playOption;
        playOption.setFont(font);
        playOption.setString("Play");
        playOption.setCharacterSize(48);
        playOption.setFillColor(sf::Color::Red); // start selected
        playOption.setPosition(windowSize.x / 2.0f - 50, windowSize.y / 2.0f);
        options.push_back(playOption);
        
        sf::Text exitOption;
        exitOption.setFont(font);
        exitOption.setString("Exit");
        exitOption.setCharacterSize(48);
        exitOption.setFillColor(sf::Color::White);
        exitOption.setPosition(windowSize.x / 2.0f - 50, windowSize.y / 2.0f + 80);
        options.push_back(exitOption);
        
        selectedIndex = 0;
    };

    // Draw the menu items
    void render(sf::RenderWindow& window) {
        window.draw(title);
        for (auto& text : options)
            window.draw(text);
    }
    
    // Process keyboard events to navigate the menu
    void moveUp() {
        if (selectedIndex > 0) {
            options[selectedIndex].setFillColor(sf::Color::White);
            selectedIndex--;
            options[selectedIndex].setFillColor(sf::Color::Red);
        }
    }
    
    void moveDown() {
        if (selectedIndex < options.size() - 1) {
            options[selectedIndex].setFillColor(sf::Color::White);
            selectedIndex++;
            options[selectedIndex].setFillColor(sf::Color::Red);
        }
    }
    
    int getSelectedIndex() const { return selectedIndex; }
    
private:
    sf::Text title;
    std::vector<sf::Text> options;
    int selectedIndex;
};

#endif // MENU_HPP
