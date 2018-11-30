#include "Space.h"

//!Space
/*! Floor like, animated tile.*/
Space::Space(std::vector<ManagedSprite*>* sprite, int x, int y)
{
	m_spaceTile = rand() % 3;

	m_solid = false;

	m_sprite = sprite;
	m_position.x = x * 32;
	m_position.y = y * 32;
	if (m_spaceTile > 2)
	{
		m_spaceTile = 2;
	}
}


void Space::Draw(sf::RenderWindow & r)
{
	m_sprite->at(m_spaceTile)->GetSprite()->setPosition(m_position);
	r.draw(*m_sprite->at(m_spaceTile)->GetSprite());
}
