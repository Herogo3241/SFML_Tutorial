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

    Player(sf::Vector2f position);
    void update(float deltaTime, const sf::Keyboard& keyboard, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, const sf::RenderWindow& window);
    void handleShooting(std::vector<Bullet>& bullets) const;
	int takeDamage(int health, Enemy& enemy);


private:
    float interpolateAngle(float current, float target, float speed);
};

#endif // PLAYER_H
