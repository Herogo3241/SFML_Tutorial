#include "Game.h"
#include <random>
#include <cmath>

Game::Game()
    : window(sf::VideoMode::getFullscreenModes()[0], "Game", sf::Style::Fullscreen),
    camera(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)),
    player(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2)),
    score(0), health(200),
    currentHealthWidth(400.0f), targetHealthWidth(400.0f), smoothness(0.1f)
{
    window.setFramerateLimit(60);
    background.setSize(sf::Vector2f(window.getSize().x * 2, window.getSize().y * 2));
    background.setFillColor(sf::Color(100, 100, 250));

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

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
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
        std::uniform_int_distribution<int> xDist(0, window.getSize().x - 10.f);
        std::uniform_int_distribution<int> yDist(0, window.getSize().y - 10.f);

        float positionX = xDist(gen);
        float positionY = yDist(gen);

        if (std::hypot(positionX - player.shape.getPosition().x, positionY - player.shape.getPosition().y) < 100.0f) {
            continue;
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
                    score += 10;
                    return true;
                }
            }
            return false;
        }), bullets.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.isAlive(); }), enemies.end());
}

void Game::update(float deltaTime) {
    camera.setCenter(player.shape.getPosition());
    background.setPosition(player.shape.getPosition() * 0.5f);

    player.update(deltaTime, sf::Keyboard(), bullets, enemies, window);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, player.shape.getPosition());
    }

    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }

    handleCollisions();
    updateHealthBar();

    scoreText.setString("Score: " + std::to_string(score));
}

void Game::render() {
    window.clear(sf::Color::Black);
    //window.draw(background);
    window.draw(player.shape);
    for (const auto& enemy : enemies) {
        window.draw(enemy.shape);
    }
    for (const auto& bullet : bullets) {
        window.draw(bullet.shape);
    }
    window.draw(scoreText);
    window.draw(healthBarBackground);
    if (currentHealthWidth > 20) {
        window.draw(leftEdge);
        window.draw(rightEdge);
    }
    window.draw(healthBarForeground);
    window.display();
}

bool Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                return false;
            }
        }

        float deltaTime = clock.restart().asSeconds();
        spawnEnemies();
        update(deltaTime);
        render();

        if (health <= 0) {
            return false; 
        }
    }

    return false;
}


void Game::reset() {
	player = Player(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	bullets.clear();
	enemies.clear();
	score = 0;
	health = 200;
	currentHealthWidth = 400.0f;
	targetHealthWidth = 400.0f;
	smoothness = 0.1f;
}

