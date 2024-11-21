#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Game.h"
#include "GameOverScreen.h"

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
    sf::SoundBuffer buffer;
	sf::Sound sound;
    sf::Font font;
    std::vector<BackgroundShape> backgroundShapes;
    sf::RectangleShape gradientOverlay;
    sf::Text howToPlayText;
    sf::Text howToPlayInstructions;
    bool isHowToPlayVisible = false;
    sf::RectangleShape howToPlayBackground;
    sf::Text optionsText;
    sf::RectangleShape optionsBackground;
    sf::Text volumeText;
    sf::RectangleShape volumeSliderBackground;
    sf::RectangleShape volumeSlider;
    bool isOptionsVisible = false;
    float currentVolume = 0.5f;

public:
    sf::RectangleShape background;
    MainMenu(sf::RenderWindow& window);
    bool run(sf::RenderWindow& window, Game& game, GameOverScreen& gameOver);  // Handle menu logic
};

#endif
