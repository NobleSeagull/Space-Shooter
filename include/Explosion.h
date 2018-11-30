#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Entity.h"
//!Explosion class
/*! explosion class deals with any entity that needs to have an explosion after getting to health '0' */
class Explosion :
	public Entity
{
public:
	Explosion(ResourceManager * r, std::vector<Entity*> * e);
	~Explosion() {};

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Position(sf::Vector2f x) { m_position = x; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };
};
#endif