#include "Collisions.h"


bool Collisions::checkCollision(sf::CircleShape& shape1, sf::CircleShape& shape2) {
	sf::Vector2f shape1Position = shape1.getPosition();
	sf::Vector2f shape2Position = shape2.getPosition();
	float distance = std::hypot(shape1Position.x - shape2Position.x, shape1Position.y - shape2Position.y);
	float radius1 = shape1.getRadius();
	float radius2 = shape2.getRadius();
	return distance < radius1 + radius2;
}

bool Collisions::checkBulletCollision(sf::CircleShape& shape1, sf::CircleShape& shape2) {
	sf::Vector2f shape1Position = shape1.getPosition();
	sf::Vector2f shape2Position = shape2.getPosition();
	float distance = std::hypot(shape1Position.x - shape2Position.x, shape1Position.y - shape2Position.y);
	float radius1 = shape1.getRadius();
	float radius2 = shape2.getRadius();
	return distance < radius1 + radius2 + 10.f;
}