#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SFML/Graphics.hpp>

class GameOverScreen {
private:
    sf::Text gameOverText;
    sf::Text restartText;
	sf::Text exitText;
    sf::Font font;

public:
    GameOverScreen();
    bool run(sf::RenderWindow& window);  // Handle game over logic
};

#endif
