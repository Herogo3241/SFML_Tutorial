#include "Game.h"
#include <random>
#include <cmath>

Game::Game()
    : window(sf::VideoMode::getFullscreenModes()[0], "Game", sf::Style::Fullscreen),
    camera(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)),
    player(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2)),
    score(0), health(200),
    currentHealthWidth(400.0f), targetHealthWidth(400.0f), smoothness(0.1f), scoreRenderer(font),
    isPaused(false)
{

	sf::Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        

    window.setFramerateLimit(60);
    background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(20, 20, 40)); 

    gradientOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    gradientOverlay.setFillColor(sf::Color(0, 0, 0, 100));

    healthBarBackground.setSize(sf::Vector2f(400, 20));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50));
    healthBarBackground.setOutlineThickness(2);
    healthBarBackground.setOutlineColor(sf::Color::White);
    healthBarBackground.setPosition(10, 40);

    healthBarForeground.setSize(sf::Vector2f(400, 20));
    healthBarForeground.setFillColor(sf::Color::Green);
    healthBarForeground.setPosition(10, 40);

    leftEdge.setRadius(10);
    leftEdge.setFillColor(sf::Color::Green);
    leftEdge.setPosition(10, 40);

    rightEdge.setRadius(10);
    rightEdge.setFillColor(sf::Color::Green);
    rightEdge.setPosition(410, 40);

    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

	sf::Vector2f scorePosition = sf::Vector2f(window.getSize().x - 600 - std::to_string(score).length(), 40);

    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(120);
    pauseText.setFillColor(sf::Color(220, 20, 60)); // Crimsion red
    pauseText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top);
    pauseText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.2f
    );

    shadowText = pauseText;
    shadowText.setFillColor(sf::Color(255, 255, 255, 50));
    shadowText.move(5, 5);

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


    resumeText.setFont(font);
    resumeText.setString("RESUME GAME");
    resumeText.setCharacterSize(48);
    resumeText.setFillColor(sf::Color::White);

    // Center restart text
    sf::FloatRect resumeBounds = resumeText.getLocalBounds();
    resumeText.setOrigin(resumeBounds.left + resumeBounds.width / 2.0f, resumeBounds.top);
    resumeText.setPosition(
        window.getSize().x / 2.0f,
        window.getSize().y * 0.5f
    );




    pauseButton.setSize(sf::Vector2f(50, 50));
    pauseButton.setFillColor(sf::Color(100, 100, 100, 200));
    pauseButton.setPosition(window.getSize().x - 100, 40);

    // handling click sound
	if (!clickBuffer.loadFromFile("click.mp3")) {
		throw std::runtime_error("Failed to load sound effect");
	}
	clickSound.setBuffer(clickBuffer);


}

void Game::updateHealthBar() {
    float healthPercentage = static_cast<float>(health) / 200.0f;
    targetHealthWidth = 400.0f * healthPercentage;
    currentHealthWidth += (targetHealthWidth - currentHealthWidth) * smoothness;

    healthBarForeground.setSize(sf::Vector2f(currentHealthWidth, 20));
    sf::Color color = calculateHealthColor(healthPercentage);
    healthBarForeground.setFillColor(color);
    leftEdge.setFillColor(color);
    rightEdge.setFillColor(color);
    rightEdge.setPosition(10 + currentHealthWidth - 20, 40);
}

sf::Color Game::calculateHealthColor(float healthPercentage) {
    return sf::Color(
        static_cast<sf::Uint8>(255 * (1.0f - healthPercentage)),
        static_cast<sf::Uint8>(255 * healthPercentage),
        0
    );
}

void Game::spawnEnemies() {
    while (enemies.size() < 30) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Extended spawn area beyond screen boundaries
        float extendedWidth = window.getSize().x + 200.f;
        float extendedHeight = window.getSize().y + 200.f;

        std::uniform_int_distribution<int> xDist(-100, extendedWidth);
        std::uniform_int_distribution<int> yDist(-100, extendedHeight);

        float positionX = xDist(gen);
        float positionY = yDist(gen);

        // Check distance from player
        float distanceToPlayer = std::hypot(
            positionX - player.shape.getPosition().x,
            positionY - player.shape.getPosition().y
        );

        // Ensure enemy spawns outside 100-pixel radius of player
        if (distanceToPlayer < 400.0f) {
            continue;
        }

        // Optional: Add screen boundary check if needed
        if (positionX < 0 || positionX > window.getSize().x ||
            positionY < 0 || positionY > window.getSize().y) {
            // Enemy spawns outside visible screen
        }

        enemies.emplace_back(sf::Vector2f(positionX, positionY));
    }
}

void Game::handleCollisions() {
    bool playerTookDamage = false;

    for (auto& enemy : enemies) {
        if (!playerTookDamage && Collisions::checkCollision(player.shape, enemy.shape)) {
            health = player.takeDamage(health, enemy);
            playerTookDamage = true;
        }
    }

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            enemies[i].handleEnemyCollision(enemies[j]);
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [this](Bullet& bullet) {
            for (auto& enemy : enemies) {
                if (Collisions::checkBulletCollision(enemy.shape, bullet.shape)) {           
                    enemy.handleCollision(100);
					enemy.burstEffect();
					enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) {return e.isDestroyed; }), enemies.end());
                    score += 5;
                    return true;
                }
            }
            return false;
        }), bullets.end());

	

 
}

void Game::update(float deltaTime) {
    camera.setCenter(player.shape.getPosition());

    
    

    player.update(deltaTime, sf::Keyboard(), bullets, enemies, window);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, player.shape.getPosition());
    }



    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }

    handleCollisions();

    
    updateHealthBar();


    scoreRenderer.updateScore(score,
        sf::Vector2f(window.getSize().x - 600 - std::to_string(score).length(), 40));
    scoreRenderer.updateParticles(deltaTime);
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(background);
    window.draw(gradientOverlay);
	player.draw(window);
    for (const auto& enemy : enemies) {
        window.draw(enemy.shape);
    }
    for (auto& bullet : bullets) {
		window.draw(bullet.shape);
    }
    scoreRenderer.draw(window);
    window.draw(healthBarBackground);
    if (currentHealthWidth > 20) {
        window.draw(leftEdge);
        window.draw(rightEdge);
    }
    window.draw(healthBarForeground);

    window.draw(pauseButton);

    // Pause icon (two vertical rectangles)
    if (!isPaused) {
        // Pause icon: two vertical bars
        sf::RectangleShape pauseBar1, pauseBar2;
        pauseBar1.setSize(sf::Vector2f(10, 30));
        pauseBar2.setSize(sf::Vector2f(10, 30));

        pauseBar1.setFillColor(sf::Color::White);
        pauseBar2.setFillColor(sf::Color::White);

        pauseBar1.setPosition(pauseButton.getPosition().x + 10, pauseButton.getPosition().y + 10);
        pauseBar2.setPosition(pauseButton.getPosition().x + 30, pauseButton.getPosition().y + 10);

        window.draw(pauseBar1);
        window.draw(pauseBar2);
    }
    else {
        // Play icon: triangle pointing right
        sf::ConvexShape playTriangle;
        playTriangle.setPointCount(3);
        playTriangle.setPoint(0, sf::Vector2f(pauseButton.getPosition().x + 15, pauseButton.getPosition().y + 10));
        playTriangle.setPoint(1, sf::Vector2f(pauseButton.getPosition().x + 15, pauseButton.getPosition().y + 40));
        playTriangle.setPoint(2, sf::Vector2f(pauseButton.getPosition().x + 40, pauseButton.getPosition().y + 25));

        playTriangle.setFillColor(sf::Color::White);

        window.draw(playTriangle);



		window.draw(pauseText);
		window.draw(shadowText);
		window.draw(exitText);
		window.draw(restartText);
		window.draw(resumeText);


    }



    window.display();
}

bool Game::run(std::string state) {
    
    while (window.isOpen()) {
        if (state != "Playing") {
            reset();
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                clickSound.play();
				isPaused = !isPaused;

                
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Handle pause button click

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (pauseButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    clickSound.play();
                    isPaused = !isPaused;
                }

                
            }

            
        }

        

        if (state == "Playing") {
            float deltaTime = clock.restart().asSeconds();

            

            if (!isPaused) {
                spawnEnemies();
                update(deltaTime);
                

                
            }
            else {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (resumeText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    resumeText.setFillColor(sf::Color(255, 215, 0)); // Gold color
                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        clickSound.play();
                        isPaused = !isPaused;
                        return true;
                        
                    }
                }
                else {
                    resumeText.setFillColor(sf::Color::White);
                }

                if (restartText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    restartText.setFillColor(sf::Color(255, 215, 0)); // Gold color
                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        clickSound.play();
                        reset();
                        isPaused = !isPaused;
                        return true;
                    }
                }
                else {
                    restartText.setFillColor(sf::Color::White);
                }

                // Handle exit text click
                if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    exitText.setFillColor(sf::Color(220, 20, 60)); // Crimson
                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        clickSound.play();
                        window.close(); 
                        return false;
                    }
                }
                else {
                    exitText.setFillColor(sf::Color::White);
                }

                
				
			}

            render();

            
            

            
        }

        if (health <= 0) {
            return false;
        }
    }


    return false;
}


void Game::reset() {
	player.reset(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	bullets.clear();
	enemies.clear();
	spawnEnemies();
	score = 0;
	health = 200;
	currentHealthWidth = 400.0f;
	targetHealthWidth = 400.0f;
	smoothness = 0.1f;
    scoreRenderer.updateScore(score,
        sf::Vector2f(window.getSize().x - 200 - std::to_string(score).length(), 40));

	
}

