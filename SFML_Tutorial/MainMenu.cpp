#include "MainMenu.h"
#include <cmath>
#include <random>


MainMenu::MainMenu(sf::RenderWindow& window) {
    // Load font with error handling
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Could not load font");
    }

    // Gradient Background
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(20, 20, 40)); // Deep dark blue-black

    // Gradient Overlay
    gradientOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    gradientOverlay.setFillColor(sf::Color(0, 0, 0, 100)); // Translucent black overlay

    // Create Animated Background Elements (Shape-themed stars/particles)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> sizeDistr(1.0, 3.0);
    std::uniform_real_distribution<> colorDistr(100, 255);
    std::uniform_real_distribution<> speedDistr(0.1, 1.0);

    for (int i = 0; i < 100; ++i) {
        sf::ConvexShape shape;
        int sides = std::rand() % 5 + 3; // 3-7 sided shapes
        shape.setPointCount(sides);

        // Create a random polygon
        for (int j = 0; j < sides; ++j) {
            float radius = sizeDistr(gen);
            float angle = j * (360.0f / sides) * (M_PI / 180.0f);
            shape.setPoint(j, sf::Vector2f(
                radius * std::cos(angle),
                radius * std::sin(angle)
            ));
        }

        // Random positioning and styling
        shape.setPosition(
            std::rand() % window.getSize().x,
            std::rand() % window.getSize().y
        );
        shape.setFillColor(sf::Color(
            colorDistr(gen),
            colorDistr(gen),
            colorDistr(gen),
            std::rand() % 100 + 50
        ));
        shape.setRotation(std::rand() % 360);

        // Store additional movement properties
        BackgroundShape bgShape;
        bgShape.shape = shape;
        bgShape.speed = speedDistr(gen);
        bgShape.rotationSpeed = (std::rand() % 20 - 10) * 0.5f;
        backgroundShapes.push_back(bgShape);
    }

    // Title Text with Shadow
    titleText.setFont(font);
    titleText.setString("SHAPE WARS!");
    titleText.setCharacterSize(120);
    titleText.setFillColor(sf::Color(50, 150, 250)); 
    titleText.setStyle(sf::Text::Bold);

    

    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top);
    titleText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.2f
    );

    // Shadow Text
    shadowText = titleText;
    shadowText.setFillColor(sf::Color(255, 255, 255, 50));
    shadowText.move(5, 5);

    // Play Text
    playText.setFont(font);
    playText.setString("START GAME");
    playText.setCharacterSize(48);
    playText.setFillColor(sf::Color::White);

    // Center play text
    sf::FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top);
    playText.setPosition(
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


    // How to Play Text
    howToPlayText.setFont(font);
    howToPlayText.setString("HOW TO PLAY");
    howToPlayText.setCharacterSize(48);
    howToPlayText.setFillColor(sf::Color::White);

    // Center how to play text
    sf::FloatRect howToPlayBounds = howToPlayText.getLocalBounds();
    howToPlayText.setOrigin(howToPlayBounds.left + howToPlayBounds.width / 2.0f, howToPlayBounds.top);
    howToPlayText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.8f
    );

    // How to Play Background
    howToPlayBackground.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.7f));
    howToPlayBackground.setFillColor(sf::Color(0, 0, 0, 200));
    howToPlayBackground.setPosition(
        window.getSize().x * 0.1f,
        window.getSize().y * 0.15f
    );

    // How to Play Instructions
    howToPlayInstructions.setFont(font);
    howToPlayInstructions.setCharacterSize(24);
    howToPlayInstructions.setFillColor(sf::Color::White);
    howToPlayInstructions.setString(
        "GAME CONTROLS:\n\n"
        "MOVEMENT:\n"
        "W - Move Up\n"
        "A - Move Left\n"
        "S - Move Down\n"
        "D - Move Right\n\n"
        "SHOOTING:\n"
        "SPACEBAR or LEFT MOUSE - Shoot\n\n"
        "OBJECTIVE:\n"
        "- Survive and destroy enemies\n"
        "- Avoid enemy collisions\n"
        "- Collect points by destroying enemies\n\n"
        "TIPS:\n"
        "- Use WASD to move and dodge\n"
        "- Aim carefully before shooting\n"
        "- Watch your health bar\n\n"
        "PRESS ESC TO PAUSE DURING GAME\n\n"
        "CLICK ANYWHERE TO CLOSE"
    );

    // Position instructions
    sf::FloatRect instructionBounds = howToPlayInstructions.getLocalBounds();
    howToPlayInstructions.setOrigin(instructionBounds.left + instructionBounds.width / 2.0f, instructionBounds.top);
    howToPlayInstructions.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.2f
    );
}

bool MainMenu::run(sf::RenderWindow& window, Game& game) {
    sf::Clock animationClock;
    float glowIntensity = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Event handling
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            // Mouse position
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            


            if (!isHowToPlayVisible) {
                // Play text interactions
                if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    playText.setFillColor(sf::Color(255, 215, 0)); // Gold color
                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
						game.reset();
                        return true; // Start game
                    }
                }
                else {
                    playText.setFillColor(sf::Color::White);
                }

                // Exit text interactions
                if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    exitText.setFillColor(sf::Color(220, 20, 60)); // Crimson
                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        window.close();
                        return false;
                    }
                }
                else {
                    exitText.setFillColor(sf::Color::White);
                }
            }

            // How to Play text interactions
            if (howToPlayText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                howToPlayText.setFillColor(sf::Color(100, 150, 250)); // Blue highlight
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    isHowToPlayVisible = true; // Show the "How to Play" overlay
                }
            }
            else {
                howToPlayText.setFillColor(sf::Color::White);
            }

            // Close How to Play overlay when clicking outside
            if (isHowToPlayVisible &&
                event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::FloatRect overlayBounds = howToPlayBackground.getGlobalBounds();

                // Only close if the click is outside the bounds of the overlay
                if (!overlayBounds.contains(mousePos.x, mousePos.y)) {
                    isHowToPlayVisible = false; // Close the "How to Play" overlay
                }
            }

        }

        // Existing animation code...
        float time = animationClock.getElapsedTime().asSeconds();
        glowIntensity = std::abs(std::sin(time * 2.0f));

        for (auto& bgShape : backgroundShapes) {
            // Move shapes
            bgShape.shape.move(0, bgShape.speed);

            // Rotate shapes
            bgShape.shape.rotate(bgShape.rotationSpeed);

            // Wrap around when off screen
            if (bgShape.shape.getPosition().y > window.getSize().y) {
                bgShape.shape.setPosition(
                    std::rand() % window.getSize().x,
                    -10.0f
                );
            }
        }

        // Drawing
        window.clear();

        // Draw background
        window.draw(background);
        window.draw(gradientOverlay);

        
        // Draw How to Play overlay if visible
        if (isHowToPlayVisible) {
            window.draw(howToPlayBackground);
            window.draw(howToPlayInstructions);
        }
        else {
            window.draw(shadowText);

            // Draw title with pulsing effect
            titleText.setFillColor(sf::Color(50, 150, 250,
                static_cast<sf::Uint8>(100 + glowIntensity * 100)));
            window.draw(titleText);

            // Draw menu options
            window.draw(playText);
            window.draw(exitText);
            window.draw(howToPlayText);

        }

        window.display();
    }
    return false;
}