#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>


#define M_PI 3.14159265358979323846


class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f position, float angle) {
        shape.setRadius(5.0f);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);

        // Calculate velocity based on the angle (convert to radians)
        float radianAngle = angle * M_PI / 180;
        velocity.x = std::cos(radianAngle) * 5.0f; // Bullet speed
        velocity.y = std::sin(radianAngle) * 5.0f;
    }

    void update(float deltaTime) {
        shape.move(velocity * deltaTime * 500.f);
    }
};





sf::Vector2f velocity(0.0f, 0.0f);
const float acceleration = 0.1f;
const float friction = 0.99f;
float currentAngle = 0.0f; // The current rotation of the triangle
const float rotationSpeed = 5.0f;
std::vector<Bullet> bullets;



void handleShooting(const sf::CircleShape& player) {
    static bool canShoot = true; // Used to manage shooting state
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot) {
        // Get the player's position and angle
        sf::Vector2f position = player.getPosition();
        float angle = player.getRotation() - 90; // Adjust for orientation

        // Create a new bullet and add it to the vector
        bullets.emplace_back(position, angle);

        canShoot = false; // Prevent continuous shooting
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        canShoot = true; // Allow shooting again when the key is released
    }
}


float interpolateAngle(float current, float target, float speed) {
    float delta = target - current;

    // Ensure the shortest path is chosen
    if (delta > 180.0f) {
        delta -= 360.0f;
    }
    else if (delta < -180.0f) {
        delta += 360.0f;
    }

    // Smoothly move towards the target angle
    return current + delta * speed;
}

static void updatePlayerPosition(sf::CircleShape& player, const sf::Keyboard& keyboard, float deltaTime) {
    if (keyboard.isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::S)) {
        velocity.y += acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= acceleration;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::D)) {
        velocity.x += acceleration;
    }

    // Apply friction to simulate space-like movement
    velocity *= friction;

    // Update the player's position
    player.move(velocity * deltaTime * 100.f);
    float targetAngle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    // Only update rotation if there's significant movement
    if (std::hypot(velocity.x, velocity.y) > 0.1f) {
        // Smoothly interpolate the current angle towards the target angle
        currentAngle = interpolateAngle(currentAngle, targetAngle + 90, rotationSpeed * 0.01f);
        player.setRotation(currentAngle);
    }
}



int main()
{

    

    

    

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Dino Game", sf::Style::Fullscreen | sf::Style::Close, settings);

    sf::CircleShape player(20, 3);

	player.setOrigin(player.getRadius(), player.getRadius());
	player.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	player.setFillColor(sf::Color::Black);
    player.setOutlineThickness(2);
    player.setOutlineColor(sf::Color::White);
	









    sf::Clock clock;

    sf::Event event;
	sf::Keyboard keyboard;
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            }

        }


        float deltaTime = clock.restart().asSeconds();
        

		updatePlayerPosition(player, keyboard, deltaTime);
        handleShooting(player);
       

        
        for (auto& bullet : bullets) {
            bullet.update(deltaTime);
        }
        
      

	
        window.clear(sf::Color::Black);
		window.draw(player);
        for (const auto& bullet : bullets) {
            window.draw(bullet.shape);
        }
 


        window.display();
    }

    return 0;
}