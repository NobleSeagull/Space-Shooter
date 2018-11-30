#include "Nest.h"


//!Nest
/*! Produces aliens to attack the player.*/
Nest::Nest(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, Graph * g, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_entity = e;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("nest"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.SetAnimationStates(0, 2);
	m_sprite.AnimateOn();

	m_explosion = explosion;

	m_type = "Nest";

	m_alive = true;

	m_health = 5;

	//!Initialize Spawn
	/*! Creates it's children pre runtime to prevent delay.*/
	for (int i = 0; i < 2; i++)
	{
		m_predators.push_back(new Predator(r, e, explosion, g, -1000, -1000));
		m_predators.back()->m_alive = false;
		m_entity->push_back(m_predators.back());
	}
	for (int i = 0; i < 2; i++)
	{
		m_missiles.push_back(new Missile(r, e, explosion,-1000, -1000));
		m_missiles.back()->m_alive = false;
		m_entity->push_back(m_missiles.back());
	}
}

//!Update - Spawn
/*! Spawns it's units. A missile every 5 seconds and a predator every 10 seconds.*/
void Nest::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		if (m_birthTimer <= 0)
		{
			m_birthTimer = 600;
			for (int i = 0; i < m_predators.size(); i++)
			{
				if (!m_predators.at(i)->m_alive)
				{
					m_predators.at(i)->m_alive = true;
					m_predators.at(i)->m_position.x = m_position.x + 16;
					m_predators.at(i)->m_position.y = m_position.y + 16;
					m_predators.at(i)->m_health = 2;
					break;
				}
			}
		}
		else
		{
			m_birthTimer--;
		}
		if (m_missileTimer <= 0)
		{
			m_missileTimer = 300;
			for (int i = 0; i < m_predators.size(); i++)
			{
				if (!m_missiles.at(i)->m_alive && m_missiles.at(i)->m_explosionTimer <= 0)
				{
					m_missiles.at(i)->Fire(m_position.x+16, m_position.y+16);
					break;
				}
			}
		}
		else
		{
			m_missileTimer--;
		}
	}
	if (m_health <= 0 && m_alive)
	{
		m_explosionTimer = 120;
		m_alive = false;
	}
	else if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosionTimer--;
	}
	else if (m_explosionTimer <= 0 && !m_alive)
	{
		m_position.x = -1000;
		m_position.y = -1000;
		m_sprite.GetSprite()->setPosition(m_position);
	}
	m_sprite.GetSprite()->setPosition(m_position);
	m_sprite.Update();
}

void Nest::Draw(sf::RenderWindow & r)
{
	if (m_alive)
	{
		m_sprite.GetSprite()->setRotation(m_orientation);
		r.draw(*m_sprite.GetSprite());
	}
	if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosion->m_position = m_position;
		m_explosion->Draw(r);
	}
}