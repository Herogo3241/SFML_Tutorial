#include "GameOverScreen.h"
#include <cmath>

GameOverScreen::GameOverScreen(sf::RenderWindow& window) {
    // Load font with error handling
    if (!font.loadFromFile("arial.ttf")) {
        // Fallback to system font or throw an error
        throw std::runtime_error("Could not load font");
    }

    // Gradient Background
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(20, 20, 40)); // Deep dark blue-black

    // Create a subtle gradient overlay
    gradientOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    gradientOverlay.setFillColor(sf::Color(0, 0, 0, 100)); // Translucent black overlay

    // Animated Background Elements
    for (int i = 0; i < 50; ++i) {
        sf::CircleShape star;
        star.setRadius(std::rand() % 2 + 1);
        star.setFillColor(sf::Color(255, 255, 255, std::rand() % 100 + 50));
        star.setPosition(
            std::rand() % window.getSize().x,
            std::rand() % window.getSize().y
        );
        backgroundStars.push_back(star);
    }

    // Game Over Text with Shadow and Glow Effect
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(120);
    gameOverText.setFillColor(sf::Color(220, 20, 60)); // Crimsion red
    gameOverText.setStyle(sf::Text::Bold);

    // Center the text horizontally
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top);
    gameOverText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.2f
    );

    // Shadow Text
    shadowText = gameOverText;
    shadowText.setFillColor(sf::Color(255, 255, 255, 50));
    shadowText.move(5, 5);

    // Restart Text with Hover Effect
    restartText.setFont(font);
    restartText.setString("RESTART GAME");
    restartText.setCharacterSize(48);
    restartText.setFillColor(sf::Color::White);

    // Center restart text
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setOrigin(restartBounds.left + restartBounds.width / 2.0f, restartBounds.top);
    restartText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.6f
    );

    // Exit Text
    exitText.setFont(font);
    exitText.setString("EXIT GAME");
    exitText.setCharacterSize(48);
    exitText.setFillColor(sf::Color::White);

    // Center exit text
    sf::FloatRect exitBounds = exitText.getLocalBounds();
    exitText.setOrigin(exitBounds.left + exitBounds.width / 2.0f, exitBounds.top);
    exitText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.7f
    );

    scoreText.setFont(font);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color(50, 200, 255)); // Vibrant blue
    scoreText.setStyle(sf::Text::Bold);

    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top);
    scoreText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.4f
    );

    // Click Sound
	buffer.loadFromFile("click.mp3");
	sound.setBuffer(buffer);

}

bool GameOverScreen::run(sf::RenderWindow& window, Game& game) {
    // Animation variables
    float glowIntensity = 0.0f;
    bool glowDirection = true;
    sf::Clock animationClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

           
            

            // Mouse position and interaction
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Restart text hover and click
            if (restartText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                restartText.setFillColor(sf::Color(255, 215, 0)); // Gold color
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
					sound.play();
                    return true; // Restart game
                }
            }
            else {
                restartText.setFillColor(sf::Color::White);
            }

            // Exit text hover and click
            if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                exitText.setFillColor(sf::Color(220, 20, 60)); // Crimson
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    sound.play();
                    window.close();
                    return false;
                }
            }
            else {
                exitText.setFillColor(sf::Color::White);
            }


            
        }

        scoreText.setString("SCORE: " + std::to_string(game.score));

        // Recalculate origin to keep it centered
        sf::FloatRect scoreBounds = scoreText.getLocalBounds();
        scoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top);
        scoreText.setPosition(
            window.getSize().x / 2.0f,
            window.getSize().y * 0.4f
        );

        // Animated glow effect for Game Over text
        float time = animationClock.getElapsedTime().asSeconds();
        glowIntensity = std::abs(std::sin(time * 2.0f));

        // Animate background stars
        for (auto& star : backgroundStars) {
            star.move(0, 0.5f); // Slow downward movement

            // Wrap stars around when they go off screen
            if (star.getPosition().y > window.getSize().y) {
                star.setPosition(
                    std::rand() % window.getSize().x,
                    -10.0f
                );
            }
        }

        // Clear and draw
        window.clear();

        // Draw background elements
        window.draw(background);

        // Draw animated stars
        /*for (const auto& star : backgroundStars) {
            window.draw(star);
        }*/

        window.draw(gradientOverlay);

        // Draw shadow text with slight offset
        window.draw(shadowText);

        // Draw Game Over text with glow
        gameOverText.setFillColor(sf::Color(220, 20, 60,
            static_cast<sf::Uint8>(100 + glowIntensity * 100)));
        window.draw(gameOverText);

        // Draw menu options
        window.draw(restartText);
        window.draw(exitText);

		window.draw(scoreText);

        window.display();
    }
    return false;
}