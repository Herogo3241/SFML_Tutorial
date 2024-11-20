#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() {
    font.loadFromFile("arial.ttf");

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(300, 200);

    restartText.setFont(font);
    restartText.setString("Press Enter to Restart");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(300, 400);

	exitText.setFont(font);
	exitText.setString("Press Escape to Exit");
	exitText.setCharacterSize(24);
	exitText.setFillColor(sf::Color::White);
	exitText.setPosition(300, 500);

}

bool GameOverScreen::run(sf::RenderWindow& window) {
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
                return true;  // Restart game
            }
        }

        window.clear();
        window.draw(gameOverText);
        window.draw(restartText);
		window.draw(exitText);
        window.display();
    }
    return false;  // Window closed
}
