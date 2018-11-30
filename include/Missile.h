#ifndef MISSILE_H
#define MISSILE_H

#include "Entity.h"
#include "Explosion.h"
//!Missile Class
/*! Deals with firing the homing missiles that use seek to follow the player and can only be used by the nests that spawn predators*/
/*! The missile itself is an enemy but has a short lifespan. Uses seek to hunt down the player at short range.*/
class Missile : public Entity
{
public:
	Missile(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, int x, int y);
	~Missile() {};

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Fire(int x, int y);
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };

private:

	const float m_maxspeed = 50;
	const float m_maxAcceleration = 50.0f;

	Explosion * m_explosion;
	std::vector<Entity*>* m_entity;
	int m_timer;
	float m_rotation;
	float m_speed;
	//!seek function
	/*! function that is similar to predator as it follows the player until it collides with the player*/
	void Seek(sf::Vector2f otherLoc, sf::Time t);

	float GetMagnitude(sf::Vector2f v)
	{
		float x = sqrt((v.x * v.x) + (v.y * v.y));
		return x;
	}

};
#endif
