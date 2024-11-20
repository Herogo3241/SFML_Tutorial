#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>



class Enemy
{
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;

	Enemy(sf::Vector2f position);
	void update(float deltaTime, const sf::Vector2f& playerPosition);
	void handleEnemyCollision(Enemy& other);
	void handleCollision(int damage);  // Apply damage from a bullet
	bool isAlive() const { return health > 0; }  // Check if the enemy is still alive
	void takeDamage(int damage);  // Reduce health when hit
	void burstEffect();  // Apply a burst effect when destroyed

private:
	int health = 100;
	bool isDestroyed = false;
	float burstRadius = 0.f;

};

#endif


