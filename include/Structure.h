#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "stdafx.h"
#include "ResourceManager.h"
#include "ManagedSprite.h"

//!Structure
/*! Parent class of all stationary objects.*/
class Structure
{
public:

	virtual void Update(sf::Time t) = 0;
	virtual void Draw(sf::RenderWindow & r) = 0;

	ResourceManager * m_resource;
	sf::Vector2f m_position;
	bool m_solid;

};
#endif