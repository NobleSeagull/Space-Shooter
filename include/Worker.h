#ifndef WORKER_H
#define WORKER_H

#include "Entity.h"
#include "Graph.h"
#include "Player.h"
//!worker class
/*! worker that uses wnader behavior and gets picked up by sweeper or player*/
class Worker : public Entity
{
public:
	Worker(ResourceManager * r, std::vector<Entity*> * e, Graph * g, int x, int y);
	~Worker() {};

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };

private:

	const float m_maxspeed = 50;
	const float m_maxAcceleration = 25.0f;
	const float m_radiusStop = 0;
	const float m_radiusSlow = 128;
	const float m_maxaccel = 10;

	float m_speed;
	float m_orientation;
	std::vector<Entity*>* m_entity;

	std::vector<Entity*> m_formation;

	Graph * m_graph;
	std::vector<Node> m_path;

	int m_pathRenewTimer;
	//! seekwander
	/*! \fn seekAndWander()
	uses something similar to seek to generate a path and then goes to through that path and chooses a new path and keeps doing this to make it look like it is randomly chooses directions to go in*/
	void seekAndWander(sf::Vector2f otherLoc, sf::Time t);

	float GetMagnitude(sf::Vector2f v)
	{
		float x = sqrt((v.x * v.x) + (v.y * v.y));

		return x;
	}
	float DistanceFormula(sf::Vector2f v1, sf::Vector2f v2)
	{
		float x = sqrt(((v2.x - v1.x)*(v2.x - v1.x)) + ((v2.y - v1.y)*(v2.y - v1.y)));

		return x;
	}

};
#endif