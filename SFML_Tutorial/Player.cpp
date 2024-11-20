#include "Player.h"
#include "Collisions.h"


Player::Player(sf::Vector2f position) : currentAngle(0.0f), velocity(0.0f, 0.0f)
{
    shape.setRadius(10.0f);
    shape.setPointCount(3);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);
}

void Player::update(float deltaTime, const sf::Keyboard& keyboard, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, const sf::RenderWindow& window)
{
    // Handle keyboard input for movement
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
    sf::Vector2f oldPosition = shape.getPosition() + sf::Vector2f(2.f, 2.f);
    velocity *= friction;

    // Update the player's position
    shape.move(velocity * deltaTime * 100.f);

    

    // Get the mouse position relative to the window
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPosition = window.mapPixelToCoords(mousePosition);

    // Compute the angle to the mouse
    sf::Vector2f playerPosition = shape.getPosition();
    float angleToMouse = std::atan2(mouseWorldPosition.y - playerPosition.y, mouseWorldPosition.x - playerPosition.x) * 180 / M_PI;

    // Check if the mouse is clicked (left button)
    
        // Smoothly rotate towards the mouse click position
        currentAngle = interpolateAngle(currentAngle, angleToMouse + 90, rotationSpeed * 0.05f);
        shape.setRotation(currentAngle);
    

    handleShooting(bullets);
 
}

void Player::handleShooting(std::vector<Bullet>& bullets) const
{
    static bool canShoot = true; // Used to manage shooting state
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && canShoot)
    {
        sf::Vector2f position = shape.getPosition();
        float angle = shape.getRotation() - 90; // Adjust for orientation

        // Create a new bullet and add it to the vector
        bullets.emplace_back(position, angle);

        canShoot = false; // Prevent continuous shooting 
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !sf::Mouse::isButtonPressed(sf::Mouse::Left)
        //&& !sf::Mouse::isButtonPressed(sf::Mouse::Left)
        )
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

int Player::takeDamage(int health, Enemy& enemy)
{
    health -= 1;
	return health;
}

