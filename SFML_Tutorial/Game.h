#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include "Bullets.h"
#include "Enemy.h"
#include "Collisions.h"
#include "ScoreRenderer.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    sf::RectangleShape background;
    sf::RectangleShape gradientOverlay;
    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    sf::CircleShape leftEdge;
    sf::CircleShape rightEdge;

    sf::Text scoreText;
    sf::Font font;
	sf::Text pauseText;
	sf::Text shadowText;

	sf::Text exitText;
	sf::Text restartText;
	sf::Text resumeText;

    ScoreRenderer scoreRenderer;

    
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

    sf::SoundBuffer clickBuffer;
    

public:
    int score;
    bool isPaused = false;
    sf::Sound clickSound;
    sf::RectangleShape pauseButton;
    sf::RectangleShape playButton;
    Game();
    bool run(std::string state);
	void reset();
    sf::RenderWindow& getWindow() { return window; };
};

#endif
