#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"
class PowerUp :
	public Entity
{
public:
	PowerUp(ResourceManager * r, std::vector<Entity*> * e, sf::View * cam, int x, int y);
	~PowerUp() {}

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };
};
#endif

