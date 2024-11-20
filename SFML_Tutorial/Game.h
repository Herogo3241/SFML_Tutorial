#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Bullets.h"
#include "Enemy.h"
#include "Collisions.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    sf::RectangleShape background;
    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    sf::CircleShape leftEdge;
    sf::CircleShape rightEdge;

    sf::Text scoreText;
    sf::Font font;

    int score;
    int health;
    float currentHealthWidth;
    float targetHealthWidth;
    float smoothness;

    sf::Clock clock;

    void updateHealthBar();
    sf::Color calculateHealthColor(float healthPercentage);
    void spawnEnemies();
    void handleCollisions();
    void update(float deltaTime);
    void render();

public:
    Game();
    bool run();
	void reset();
    sf::RenderWindow& getWindow() { return window; };
};

#endif
