#ifndef RADAR_H
#define RADAR_H

#include "ResourceManager.h"
#include "Entity.h"
#include "stdafx.h"
//!Radar
/*! tracks all workers and tells you the direction of each one so you can find them in the world and pick them up*/
class Radar
{
public:
	Radar(ResourceManager * r, std::vector<Entity*> * e, int x, int y);
	~Radar() {};
	void Update();
	void Draw(sf::RenderWindow & r);

private:

	sf::Vector2f m_position;
	std::vector<Entity*>* m_entity;
	ResourceManager * m_resource;
	std::vector<ManagedSprite> m_sprites;
	bool n, s, e, w, ne, nw, se, sw; //Directions

	float DistanceFormula(sf::Vector2f v1, sf::Vector2f v2)
	{
		float x = sqrt(((v2.x - v1.x)*(v2.x - v1.x)) + ((v2.y - v1.y)*(v2.y - v1.y)));

		return x;
	}

};
#endif

