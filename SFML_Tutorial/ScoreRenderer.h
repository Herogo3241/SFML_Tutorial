#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class ScoreRenderer {
private:
    struct ScoreParticle {
        sf::Text text;
        sf::Vector2f velocity;
        float lifetime;
    };

    sf::Font font;
    sf::Text scoreText;
    sf::Text shadowText;
    sf::Clock animationClock;
    std::vector<ScoreParticle> scoreParticles;

public:
    ScoreRenderer(const sf::Font& gameFont);
    void createScoreParticles(int score);
    void updateScore(int score, const sf::Vector2f& scorePosition);
    void updateParticles(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    
};