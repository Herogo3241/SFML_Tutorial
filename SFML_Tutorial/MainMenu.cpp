#include "MainMenu.h"

MainMenu::MainMenu() {
    font.loadFromFile("arial.ttf");

    titleText.setFont(font);
    titleText.setString("Welcome to the Game");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(300, 200);

    playText.setFont(font);
    playText.setString("Press Enter to Play");
    playText.setCharacterSize(24);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(300, 400);

    exitText.setFont(font);
    exitText.setString("Press Escape to Exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(300, 500);
}

bool MainMenu::run(sf::RenderWindow& window) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                return true;  // Start game
            }
        }

        window.clear();
        window.draw(titleText);
        window.draw(playText);
		window.draw(exitText);
        window.display();
    }
    return false;  // Window closed
}
