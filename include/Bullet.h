#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "Explosion.h"

/*! Bullet Class */
class Bullet :
	public Entity
{
public:
	/**constrcutor*/
	Bullet(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, int x, int y, std::string source);
	/**destructor*/
	~Bullet() {};
	/** Update bullet with the world*/
	void Update(sf::Time t, std::vector<Structure*>* s);
	/** draws the bullet to the world*/
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };/**< m_type of string "Bullet" */
	sf::Vector2f Position() { return m_position; };
	/**Function to fire the bullet*/
	void Fire(int x, int y, float speed, float direction);
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };

private:

	std::string m_source;
	Explosion * m_explosion;
	std::vector<Entity*> * m_entity;
	int m_timer;
	float m_rotation;
	float m_speed;
};

#endif

