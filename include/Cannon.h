#ifndef CANNON_H
#define CANNON_H

#include <math.h>
#include "ManagedSprite.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Bullet.h"
#include "Explosion.h"
//! The cannon class
/*!
the cannon class controls the cannon on top of the player and the predator and you can rotate the players using the mouse
*/
class Cannon
{

public:
	Cannon() {};
	Cannon(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, int x, int y, std::string source);
	~Cannon() {}
	//! mouse button presses 
	/*! if the left mouse button is pressed the bullet becomes alive and fires at the required fire rate */
	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	sf::Vector2f Position() { return m_position; };
	void Position(float x, float y) { m_position.x = x; m_position.y = y; };
	float Orientation() { return m_orientation; };
	void Orientation(float x) { m_orientation = x; };
	void BulletSpeed(float x) { m_bulletSpeed = x; };
	void FireRate(int x) { m_fireRate = x; };
	void ForceFire();

private:
	float m_angle;
	sf::Vector2f m_mouse;
	sf::Vector2f m_position;
	ManagedSprite m_sprite;
	Entity * m_entity;
	ResourceManager * m_resource;
	float m_orientation;
	const float PI = 3.14159265;
	std::vector<Bullet> m_bullets;

	std::string m_source;

	int m_fireRate;
	int m_fireTimer;
	float m_bulletSpeed;

	float m_dx;
	float m_dy;
};
#endif
