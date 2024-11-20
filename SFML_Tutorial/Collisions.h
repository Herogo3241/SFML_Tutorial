#ifndef COLLISIONS_H
#define COLLISIONS_H
#include <SFML/Graphics.hpp>

class Collisions
{	
public:
	static bool checkCollision(sf::CircleShape& shape1, sf::CircleShape& shape2);
	static bool checkBulletCollision(sf::CircleShape& shape1, sf::CircleShape& shape2);

};

#endif 

