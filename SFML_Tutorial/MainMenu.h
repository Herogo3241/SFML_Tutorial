#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Game.h"

#define M_PI 3.14159265358979323846

struct BackgroundShape {
    sf::ConvexShape shape;
    float speed;
    float rotationSpeed;
};

class MainMenu {
private:
    sf::Text titleText;
    sf::Text playText;
    sf::Text exitText;
    sf::Text shadowText;
    sf::Font font;
    std::vector<BackgroundShape> backgroundShapes;
    sf::RectangleShape gradientOverlay;
    sf::Text howToPlayText;
    sf::Text howToPlayInstructions;
    bool isHowToPlayVisible = false;
    sf::RectangleShape howToPlayBackground;

public:
    sf::RectangleShape background;
    MainMenu(sf::RenderWindow& window);
    bool run(sf::RenderWindow& window, Game& game);  // Handle menu logic
};

#endif
