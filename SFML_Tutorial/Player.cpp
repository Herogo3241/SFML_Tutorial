#include "Player.h"
#include "Collisions.h"


Player::Player(sf::Vector2f position) : currentAngle(0.0f), velocity(0.0f, 0.0f), isDamaged(false)
{
    shape.setRadius(10.0f);
    shape.setPointCount(5);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);
}

void Player::update(float deltaTime, const sf::Keyboard& keyboard, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, const sf::RenderWindow& window)
{
    if (isDamaged && damageClock.getElapsedTime().asSeconds() > 0.1f)
    {
        isDamaged = false;
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::White);
    }

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

    // Apply friction
    velocity *= friction;

    // Update position
    shape.move(velocity * deltaTime * 100.f);

    // Boundary constraint
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    float playerRadius = shape.getRadius();

    // Get window boundaries
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    float bufferX = 30.0f;
    float bufferY = 30.0f;

    // Constrain X-axis with extended limits
    if (shape.getPosition().x + playerRadius < -bufferX)
    {
        shape.setPosition(-bufferX - playerRadius, shape.getPosition().y);
        velocity.x = 0;
    }
    else if (shape.getPosition().x - playerRadius > windowWidth + bufferX)
    {
        shape.setPosition(windowWidth + bufferX + playerRadius, shape.getPosition().y);
        velocity.x = 0;
    }

    // Constrain Y-axis with extended limits
    if (shape.getPosition().y + playerRadius < -bufferY)
    {
        shape.setPosition(shape.getPosition().x, -bufferY - playerRadius);
        velocity.y = 0;
    }
    else if (shape.getPosition().y - playerRadius > windowHeight + bufferY)
    {
        shape.setPosition(shape.getPosition().x, windowHeight + bufferY + playerRadius);
        velocity.y = 0;
    }

    // Add position to the trail
    trailPositions.push_back(shape.getPosition());
    if (trailPositions.size() > maxTrailLength)
    {
        trailPositions.erase(trailPositions.begin());
    }

    // Mouse handling
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPosition = window.mapPixelToCoords(mousePosition);
    sf::Vector2f playerPosition = shape.getPosition();
    float angleToMouse = std::atan2(mouseWorldPosition.y - playerPosition.y, mouseWorldPosition.x - playerPosition.x) * 180 / M_PI;
    currentAngle = interpolateAngle(currentAngle, angleToMouse + 90, rotationSpeed * 0.05f);
    shape.setRotation(currentAngle);

    handleShooting(bullets);
}

void Player::draw(sf::RenderWindow& window)
{
    // Draw trail
    for (size_t i = 0; i < trailPositions.size(); ++i)
    {
        float alpha = 255 * (static_cast<float>(i) / trailPositions.size());
		float size = 10.0f * (static_cast<float>(i) / trailPositions.size());
        sf::CircleShape trailShape(size, 5); 
        trailShape.setPosition(trailPositions[i]);
        trailShape.setOrigin(trailShape.getRadius(), trailShape.getRadius());
        trailShape.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha))); // Fading effect
        window.draw(trailShape);
    }

    // Draw player
	window.draw(shape);
}


void Player::handleShooting(std::vector<Bullet>& bullets) const
{
    static bool canShoot = true; // Used to manage shooting state
    if ( sf::Mouse::isButtonPressed(sf::Mouse::Left) && canShoot)
    {
        sf::Vector2f position = shape.getPosition();
        float angle = shape.getRotation() - 90; // Adjust for orientation

        // Create a new bullet and add it to the vector
        bullets.emplace_back(position, angle);

        canShoot = false; // Prevent continuous shooting 
    }
    else if ( !sf::Mouse::isButtonPressed(sf::Mouse::Left)
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
	flashRedTint();
	return health;
}

void Player::flashRedTint()
{
    isDamaged = true;
	updateGlowEffect();
	damageClock.restart();

}


void Player::updateGlowEffect()
{
    float time = glowClock.getElapsedTime().asSeconds();

    // Create a sine wave oscillation between 0 and 1
    float pulseIntensity = (std::sin(time * 4.0f) + 1.0f) / 2.0f;

    // Base color (medium green)
    sf::Color baseColor(0, 255, 127);  // Spring green

    // Glow color (bright neon green)
    sf::Color glowColor(0, 255, 0);  

    // Interpolate between base color and glow color
    sf::Color currentColor(
        baseColor.r + (glowColor.r - baseColor.r) * pulseIntensity,
        baseColor.g + (glowColor.g - baseColor.g) * pulseIntensity,
        baseColor.b + (glowColor.b - baseColor.b) * pulseIntensity
    );

    shape.setFillColor(currentColor);

    // Create an outer glow using outline
    sf::Color outlineColor = glowColor;
    outlineColor.a = 128 + (127 * pulseIntensity); // Pulsing transparency
    shape.setOutlineThickness(5.0f + (2.0f * pulseIntensity)); // Pulsing thickness
    shape.setOutlineColor(outlineColor);
}


void Player::reset(sf::Vector2f pos) {
	shape.setPosition(pos);
	shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::White);
	velocity = sf::Vector2f(0, 0);
	currentAngle = 0;
	trailPositions.clear();
	isDamaged = false;
	damageClock.restart();
}


