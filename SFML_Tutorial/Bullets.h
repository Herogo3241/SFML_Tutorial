#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>

#define M_PI  3.14159265358979323846;

class Bullet
{
private:
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f position, float angle);
    void update(float deltaTime);

};


#endif // BULLET_H class Bullet
