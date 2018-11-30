#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"
class PowerUp :
	public Entity
{
public:
	PowerUp(ResourceManager * r, std::vector<Entity*> * e, int x, int y);
	~PowerUp() {}

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };

private:

	std::vector<Entity*> * m_entity;
	float DistanceFormula(sf::Vector2f v1, sf::Vector2f v2)
	{
		float x = sqrt(((v2.x - v1.x)*(v2.x - v1.x)) + ((v2.y - v1.y)*(v2.y - v1.y)));

		return x;
	}

};
#endif