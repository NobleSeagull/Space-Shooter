#include "Explosion.h"



Explosion::Explosion(ResourceManager * r, std::vector<Entity*> * e)
{
	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("explosion"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.SetAnimationStates(0, 2);

	m_sprite.GetSprite()->setOrigin(8, 8);
	m_sprite.AnimateOn();

	m_type = "Explosion";/*!<Type Explosion to ID*/

	m_alive = true;
}

void Explosion::Update(sf::Time t, std::vector<Structure*>* s)
{
	m_sprite.Update();
}

void Explosion::Draw(sf::RenderWindow & r)
{
	m_sprite.GetSprite()->setPosition(m_position);
	r.draw(*m_sprite.GetSprite());
}