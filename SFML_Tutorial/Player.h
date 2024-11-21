#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Bullets.h"
#include "Enemy.h"

class Player
{

public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float currentAngle;
    static constexpr float acceleration = 0.1f;
    static constexpr float friction = 0.99f;
    static constexpr float rotationSpeed = 5.0f;
    std::vector<sf::Vector2f> trailPositions; 
    const size_t maxTrailLength = 100;        

    Player(sf::Vector2f position);
    void update(float deltaTime, const sf::Keyboard& keyboard, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, const sf::RenderWindow& window);
    void handleShooting(std::vector<Bullet>& bullets) const;
    void draw(sf::RenderWindow& window);
	int takeDamage(int health, Enemy& enemy);
    void reset(sf::Vector2f pos);


private:
    sf::Clock damageClock;
    bool isDamaged;
    float interpolateAngle(float current, float target, float speed);
    void flashRedTint();
};

#endif // PLAYER_H
