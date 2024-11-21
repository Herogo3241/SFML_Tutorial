#include "ScoreRenderer.h"

ScoreRenderer::ScoreRenderer(const sf::Font& font) {
    // Main score text styling
    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color(50, 200, 255)); // Vibrant blue
    scoreText.setStyle(sf::Text::Bold);

    // Shadow text for depth
    shadowText = scoreText;
    shadowText.setFillColor(sf::Color(20, 20, 20, 150));
}

void ScoreRenderer::createScoreParticles(int score) {
    // Generate particles when score changes
    for (int i = 0; i < 10; ++i) {
        ScoreRenderer::ScoreParticle particle;

        // Particle text (could be "+" or score increment)
        particle.text.setFont(*scoreText.getFont());
        particle.text.setCharacterSize(20);
        particle.text.setString("+" + std::to_string(score / 10));
        particle.text.setFillColor(sf::Color(255, 215, 0, 200)); // Golden with transparency

        // Random initial position near score
        particle.text.setPosition(
            scoreText.getPosition().x + std::rand() % 50 - 25,
            scoreText.getPosition().y + std::rand() % 50 - 25
        );

        // Random velocity for particle movement
        particle.velocity = sf::Vector2f(
            (std::rand() % 20 - 10) * 0.5f,
            -(std::rand() % 10 + 5)
        );

        // Lifetime for particle
        particle.lifetime = 1.0f;

        scoreParticles.push_back(particle);
    }
}

void ScoreRenderer::updateScore(int score, const sf::Vector2f& scorePosition) {
    // Update main score text
    scoreText.setString("SCORE: " + std::to_string(score));

    // Positioning with slight offset for shadow effect
    scoreText.setPosition(scorePosition);
    shadowText.setString(scoreText.getString());
    shadowText.setPosition(scorePosition.x + 3, scorePosition.y + 3);

    // Create score change particles
    //createScoreParticles(score);
}



void ScoreRenderer::updateParticles(float deltaTime) {
    // Update and remove expired particles
    for (auto it = scoreParticles.begin(); it != scoreParticles.end();) {
        // Move particle
        it->text.move(it->velocity);

        // Fade out
        sf::Color color = it->text.getFillColor();
        color.a = static_cast<sf::Uint8>(color.a * 0.95f);
        it->text.setFillColor(color);

        // Reduce lifetime
        it->lifetime -= deltaTime;

        // Remove expired particles
        if (it->lifetime <= 0 || color.a < 10) {
            it = scoreParticles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ScoreRenderer::draw(sf::RenderWindow& window) {
    // Animate score text with subtle pulsing
    float time = animationClock.getElapsedTime().asSeconds();
    float scale = 1.0f + std::sin(time * 3.0f) * 0.05f;

    /*scoreText.setScale(scale, scale);
    shadowText.setScale(scale, scale);*/

    // Draw shadow first for depth effect
    window.draw(shadowText);

    // Draw main score text
    window.draw(scoreText);

    // Draw score change particles
   /* for (const auto& particle : scoreParticles) {
        window.draw(particle.text);
    }*/
}

// Getter for current score text position
sf::Vector2f ScoreRenderer::getPosition() const {
    return scoreText.getPosition();
}

