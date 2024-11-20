#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>

class MainMenu {
private:
    sf::Text titleText;
    sf::Text playText;
	sf::Text exitText;
    sf::Font font;

public:
    MainMenu();
    bool run(sf::RenderWindow& window);  // Handle menu logic
};

#endif
