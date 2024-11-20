#include "Enemy.h"
#include <cmath>
#include <random>

Enemy::Enemy(sf::Vector2f position) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(2);
    shape.setPosition(position);
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPosition) {
    if (isDestroyed) {
        // Handle burst animation after destruction
        burstEffect();
    }
    else {
        // Regular movement towards player
        sf::Vector2f direction = playerPosition - shape.getPosition();
        if (direction.x != 0.f || direction.y != 0.f) {
            direction = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);  // Normalize
            shape.move(direction * deltaTime * 100.f);  // Speed of movement
        }
    }
}

void Enemy::handleEnemyCollision(Enemy& other)
{
    // Calculate the direction from this enemy to the other
    sf::Vector2f direction = shape.getPosition() - other.shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);


    float gap = 20.f;

    // Sum of radii plus random gap
    float radiusSum = shape.getRadius() + other.shape.getRadius() + gap;

    // If the distance is less than the sum of their radii, a collision has occurred
    if (distance < radiusSum) {
        // Calculate overlap
        float overlap = radiusSum - distance;

        // Normalize the direction vector to move the enemies apart
        if (distance != 0.f) {
            direction /= distance;  // Normalize the direction vector

            // Apply the overlap to move enemies apart
            shape.move(direction * overlap * 0.5f);   // Move this enemy
            other.shape.move(-direction * overlap * 0.5f); // Move the other enemy
        }
    }
}


void Enemy::takeDamage(int damage) {
    if (isDestroyed) return;  // If the enemy is already destroyed, ignore further damage

    health -= damage;  // Decrease health
    if (health <= 0) {
        isDestroyed = true;  // Mark the enemy as destroyed when health reaches 0
    }
}

void Enemy::burstEffect() {
    if (burstRadius < 50.f) {  // Limit the size of the burst
        burstRadius += 1.f;  // Increase the burst radius
        shape.setRadius(burstRadius);  // Increase the size of the shape
        shape.setFillColor(sf::Color::Yellow);  // Change color for burst effect
    }
}

void Enemy::handleCollision(int damage) {
    takeDamage(damage);  // Apply damage when a collision happens
}
