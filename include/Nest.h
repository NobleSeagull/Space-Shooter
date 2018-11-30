#ifndef NEST_H
#define NEST_H

#include "Entity.h"
#include "Predator.h"
#include "Missile.h"


//!Nest Class
/*! Nest class spawns the predators when there is not enough predators and also fires guided missiles while the player is in range*/
class Nest : public Entity
{
public:
	Nest(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, Graph * g, int x, int y);
	~Nest() {};

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };

private:
	std::vector<Entity*> m_predators;
	std::vector<Entity*>* m_entity;
	std::vector<Missile*> m_missiles;

	Explosion * m_explosion;

	int m_birthTimer;
	int m_missileTimer;
};
#endif
