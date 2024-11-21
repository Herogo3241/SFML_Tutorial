#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Game.h"

class GameOverScreen {
private:
	sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;
	sf::Text exitText;
	sf::Text shadowText;
    sf::Font font;
	std::vector<sf::CircleShape> backgroundStars;
	sf::RectangleShape gradientOverlay;	
	sf::Vector2f windowSize;
	sf::Vector2f exitBounds;
	sf::Vector2f star;
	sf::Vector2f time;

public:
    GameOverScreen(sf::RenderWindow& window);
    bool run(sf::RenderWindow& window, Game& game);  // Handle game over logic
    sf::RectangleShape background;
};

#endif
