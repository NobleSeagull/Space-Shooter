#include "Missile.h"


//!Graph
/*! Creates a graph that manages pathfinding.*/
Missile::Missile(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_entity = e;
	m_explosion = explosion;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("missile"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.SetIdle(0);
	m_sprite.SetAnimationStates(0, 1);
	m_sprite.AnimateOn();

	m_sprite.GetSprite()->setOrigin(8, 8);

	m_velocity.x = 0;
	m_velocity.y = 0;

	m_explosion = explosion;

	m_type = "Missile";/*!<type Missile to ID*/
	m_rotation = 0;
	m_alive = false;
	m_health = 1;
}

void Missile::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		if (m_timer <= 0)
		{
			m_health = 0;
		}
		else
		{
			m_timer--;
		}

		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Player")
			{
				Seek(m_entity->at(i)->Position(), t);
			}
		}

		for (int i = 0; i < s->size(); i++)
		{
			sf::Vector2f tempPos = sf::Vector2f(m_position.x - 32, m_position.y - 32);
			tempPos.x = tempPos.x + (m_velocity.x * (float)t.asSeconds());
			sf::Rect<int> tempRect1 = sf::Rect<int>(tempPos.x, tempPos.y, 32, 32);
			sf::Rect<int> tempRect2 = sf::Rect<int>(s->at(i)->m_position.x - 16, s->at(i)->m_position.y - 16, 32, 32);

			sf::Vector2f tempPos2 = sf::Vector2f(m_position.x - 32, m_position.y - 32);
			tempPos2.y = tempPos2.y + (m_velocity.y * (float)t.asSeconds());
			sf::Rect<int> tempRect3 = sf::Rect<int>(tempPos2.x, tempPos2.y, 32, 32);
			sf::Rect<int> tempRect4 = sf::Rect<int>(s->at(i)->m_position.x - 16, s->at(i)->m_position.y - 16, 32, 32);
			if (tempRect1.intersects(tempRect2))
			{
				m_velocity.x = 0;
			}
			else if (tempRect3.intersects(tempRect4))
			{
				m_velocity.y = 0;
			}
		}

		m_position = m_position + (m_velocity * (float)t.asSeconds());

		m_rotation++;
		m_sprite.Update();

		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Player")
			{
				sf::Vector2f tempPos = sf::Vector2f(m_position.x - 16, m_position.y - 16);
				sf::Rect<int> tempRect1 = sf::Rect<int>(tempPos.x, tempPos.y, 32, 32);
				sf::Rect<int> tempRect2 = sf::Rect<int>(m_entity->at(i)->Position().x - 10, m_entity->at(i)->Position().y - 10, 20, 20);
				if (tempRect1.intersects(tempRect2))
				{
					m_entity->at(i)->Health(m_entity->at(i)->Health() - 1);
					m_health = 0;
				}
			}
		}
	}
	if (m_health <= 0 && m_alive)
	{
		m_explosionTimer = 120;
		m_alive = false;
	}
	else if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosion->Position(sf::Vector2f(m_position.x - 16, m_position.y - 16));
		m_explosionTimer--;
	}
	else if (m_explosionTimer <= 0 && !m_alive)
	{
		m_position.x = -1000;
		m_position.y = -1000;
		m_sprite.GetSprite()->setPosition(m_position);
	}
}

void Missile::Draw(sf::RenderWindow & r)
{
	if (m_alive)
	{
		m_sprite.GetSprite()->setPosition(m_position);
		m_sprite.GetSprite()->setRotation(m_rotation);
		r.draw(*m_sprite.GetSprite());
	}
	if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosion->m_position = m_position;
		m_explosion->Draw(r);
	}
}

void Missile::Fire(int x, int y)
{
	m_velocity.x = 0;
	m_velocity.y = 0;
	m_position.x = x;
	m_position.y = y;
	m_alive = true;
	m_timer = 600;
	m_health = 1;
}

//!Seek
/*! Seeks a down target based on straightline distance. Modified to always assure maximum speed and minimum control!*/
void Missile::Seek(sf::Vector2f otherLoc, sf::Time t)
{
	sf::Vector2f m_linear = otherLoc - m_position;

	m_linear.x = m_linear.x / GetMagnitude(m_linear);
	m_linear.y = m_linear.y / GetMagnitude(m_linear);

	m_linear = m_linear * m_maxAcceleration;

	m_velocity = m_velocity + (m_linear * (float)t.asSeconds());
	if (m_velocity.x > m_maxspeed)
	{
		m_velocity.x = m_maxspeed;
	}
	else if (m_velocity.x < -m_maxspeed)
	{
		m_velocity.x = -m_maxspeed;
	}

	if (m_velocity.y > m_maxspeed)
	{
		m_velocity.y = m_maxspeed;
	}
	else if (m_velocity.y < -m_maxspeed)
	{
		m_velocity.y = -m_maxspeed;
	}
}