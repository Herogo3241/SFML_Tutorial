#include "Bullets.h"

#define M_PI 3.14159265358979323846

Bullet::Bullet(sf::Vector2f position, float angle)
{
    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);

    // Calculate velocity based on the angle (convert to radians)
    float radianAngle = angle * M_PI / 180;
    velocity.x = std::cos(radianAngle) * 5.0f; // Bullet speed
    velocity.y = std::sin(radianAngle) * 5.0f;
}

void Bullet::update(float deltaTime)
{
    shape.move(velocity * deltaTime * 500.f);
}


