#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullets.h"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Dino Game", sf::Style::Fullscreen | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    sf::View camera(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

    sf::RectangleShape background(sf::Vector2f(window.getSize().x * 2, window.getSize().y * 2));
    background.setFillColor(sf::Color(100, 100, 250));

    Player player(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    std::vector<Bullet> bullets;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime, sf::Keyboard(), bullets);

        camera.setCenter(player.shape.getPosition());
        sf::Vector2f backgroundPosition = player.shape.getPosition() * 0.5f;
        background.setPosition(backgroundPosition);
        window.setView(camera);

        for (auto& bullet : bullets)
        {
            bullet.update(deltaTime);
        }

        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(player.shape);
        for (const auto& bullet : bullets)
        {
            window.draw(bullet.shape);
        }
        window.display();
    }

    return 0;
}
