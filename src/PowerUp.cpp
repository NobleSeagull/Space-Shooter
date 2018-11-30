#include "PowerUp.h"

PowerUp::PowerUp(ResourceManager * r, std::vector<Entity*> * e, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_entity = e;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("powerup"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.SetAnimationStates(0, 1);
	m_sprite.AnimateOn();

	m_sprite.GetSprite()->setOrigin(8, 8);

	m_type = "Powerup";

	m_health = 1;

	m_alive = true;
}

void PowerUp::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Player")
			{
				if (DistanceFormula(m_entity->at(i)->Position(), m_position) < 32)
				{
					m_alive = false;
					m_health = 0;
					m_position.x = -1000;
					m_position.y = -1000;
					m_entity->at(i)->m_fireRate -= 5;
				}
			}
		}

		m_sprite.GetSprite()->setPosition(m_position);
		m_sprite.Update();
	}
}

void PowerUp::Draw(sf::RenderWindow & r)
{
	if (m_alive)
	{
		m_sprite.GetSprite()->setRotation(m_orientation);
		r.draw(*m_sprite.GetSprite());
	}
}