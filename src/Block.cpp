#include "Block.h"



Block::Block(ManagedSprite * sprite, int x, int y, bool solid)
{
	m_solid = solid;
	m_sprite = sprite;
	m_position.x = x * 32;
	m_position.y = y * 32;
}

void Block::Draw(sf::RenderWindow & r)
{
	m_sprite->GetSprite()->setPosition(m_position);
	r.draw(*m_sprite->GetSprite());
}
