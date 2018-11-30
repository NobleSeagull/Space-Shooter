#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"
#include "ResourceManager.h"
#include "Graph.h"
#include "Structure.h"
#include "ManagedSprite.h"

//!Entity
/*! Parent class of all moving objects*/
class Entity
{
public:

	virtual void Update(sf::Time t, std::vector<Structure*>* s) = 0;
	virtual void Draw(sf::RenderWindow & r) = 0;
	virtual std::string Type() = 0;
	virtual sf::Vector2f Position() = 0;
	virtual void Health(int x) = 0;
	virtual int Health() = 0;

	float m_orientation;
	std::vector<Entity*> m_formation;
	ResourceManager * m_resource;
	ManagedSprite m_sprite;
	bool m_leader = false;
	bool m_isSlave = false;
	Entity* m_master;
	std::vector<Node> m_path;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	int m_explosionTimer;
	std::string m_type;
	bool m_alive;

	int m_fireRate;

	int m_score;

	int m_health;

};
#endif