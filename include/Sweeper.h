#ifndef SWEEPER_H
#define SWEEPER_H

#include "Entity.h"
#include "Graph.h"
#include "Player.h"
#include "Worker.h"

//!Sweeper
/*! Hunts for civilians and wisks them away in his mimicing shuttle.*/
class Sweeper : public Entity
{
public:
	Sweeper(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, Graph * g, int x, int y);
	~Sweeper();

	void Update(sf::Time t, std::vector<Structure*>* s);
	void Draw(sf::RenderWindow & r);
	std::string Type() { return m_type; };
	sf::Vector2f Position() { return m_position; };
	void Health(int x) { m_health = x; };
	int Health() { return m_health; };


private:
	const float m_maxspeed = 100;
	const float m_maxAcceleration = 25.0f;
	const float m_radiusStop = 16;
	const float m_radiusSlow = 32;
	const float m_maxAccel = 25.0f;

	bool m_seeking;

	float m_speed;
	std::vector<Entity*> * m_entity;
	Explosion * m_explosion;

	Graph * m_graph;

	int m_pathRenewTimer;

	void PursueWorker(sf::Vector2f otherLocation, sf::Time t);
	void Flee(sf::Time t, sf::Vector2f otherLocation);

	int m_countWorkers;

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
