#include "Player.h"

Player::Player(sf::Vector2f position) : currentAngle(0.0f), velocity(0.0f, 0.0f)
{
    shape.setRadius(10.0f);
    shape.setPointCount(3);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Black);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);
}

void Player::update(float deltaTime, const sf::Keyboard& keyboard, std::vector<Bullet>& bullets)
{
    if (keyboard.isKeyPressed(sf::Keyboard::W))
    {
        velocity.y -= acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::S))
    {
        velocity.y += acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::A))
    {
        velocity.x -= acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::D))
    {
        velocity.x += acceleration;
    }

    // Apply friction to simulate space-like movement
    velocity *= friction;

    // Update the player's position
    shape.move(velocity * deltaTime * 100.f);
    float targetAngle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    // Only update rotation if there's significant movement
    if (std::hypot(velocity.x, velocity.y) > 0.1f)
    {
        // Smoothly interpolate the current angle towards the target angle
        currentAngle = interpolateAngle(currentAngle, targetAngle + 90, rotationSpeed * 0.01f);
        shape.setRotation(currentAngle);
    }

    handleShooting(bullets);
}

void Player::handleShooting(std::vector<Bullet>& bullets)
{
    static bool canShoot = true; // Used to manage shooting state
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot)
    {
        sf::Vector2f position = shape.getPosition();
        float angle = shape.getRotation() - 90; // Adjust for orientation

        // Create a new bullet and add it to the vector
        bullets.emplace_back(position, angle);

        canShoot = false; // Prevent continuous shooting
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        canShoot = true; // Allow shooting again when the key is released
    }
}

float Player::interpolateAngle(float current, float target, float speed)
{
    float delta = target - current;

    // Ensure the shortest path is chosen
    if (delta > 180.0f)
    {
        delta -= 360.0f;
    }
    else if (delta < -180.0f)
    {
        delta += 360.0f;
    }

    // Smoothly move towards the target angle
    return current + delta * speed;
}
