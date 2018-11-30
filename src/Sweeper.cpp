#include "Sweeper.h"

Sweeper::Sweeper(ResourceManager * r, std::vector<Entity*>* e, Explosion * explosion, Graph * g, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_graph = g;
	m_entity = e;

	m_explosion = explosion;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("player"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.GetSprite()->setOrigin(8, 8);
	m_sprite.SetAnimationStates(0, 1);
	m_sprite.AnimateOn();

	m_type = "Sweeper";

	m_speed = 0;
	m_pathRenewTimer = 0;
	m_alive = true;
	m_health = 2;
	m_countWorkers = 0;

}

Sweeper::~Sweeper(){}

//!Update
/*! Hunts civilians and abducts them. If killed, offers player all of the civilians.*/
void Sweeper::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		if (m_pathRenewTimer <= 0)
		{
			for (int i = 0; i < m_entity->size(); i++)
			{
				if (DistanceFormula(m_entity->at(i)->Position(), m_position) < 32 && m_entity->at(i)->Type() == "Worker")
				{
					m_countWorkers += 1;
					m_entity->at(i)->m_alive = false;
					m_entity->at(i)->m_position.x = -1000;
					m_entity->at(i)->m_position.y = -1000;
					m_entity->at(i)->m_health = 0;

					std::cout << "captured worker" + m_countWorkers << std::endl;
					break;

				}
				else if(m_entity->at(i)->Type() == "Player" && DistanceFormula(m_entity->at(i)->Position(), m_position) < 32 * 5)
				{
					Flee(t, m_entity->at(i)->m_position);
				}
				else if (m_entity->at(i)->Type() == "Worker" && m_entity->at(i)->m_alive && m_entity->at(i) != this)
				{
					if (DistanceFormula(m_entity->at(i)->Position(), m_position) > 32 * 3)
					{
						m_path = m_graph->Path((int)(m_entity->at(i)->Position().x / 32), (int)(m_entity->at(i)->Position().y / 32), (int)(m_position.x / 32), (int)(m_position.y / 32));
					}

				}
				m_pathRenewTimer = 150;
			}
		}
		m_pathRenewTimer--;

		if (m_path.size() > 0)
		{
			sf::Rect<int> holderThis = sf::Rect<int>(m_position.x - 16, m_position.y - 16, 32, 32);
			sf::Rect<int> holderOther = sf::Rect<int>((m_path.back().x * 32) - 24, (m_path.back().y * 32) - 24, 48, 48);

			if (holderThis.intersects(holderOther))
			{
				if (m_path.size() > 1)
				{
					m_path.pop_back();
				}
			}
			PursueWorker(sf::Vector2f(m_path.back().x * 32, m_path.back().y * 32), t);
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
				m_velocity.x = (m_velocity.x * -1) / 2;

			}
			else if (tempRect3.intersects(tempRect4))
			{
				m_velocity.y = (m_velocity.y * -1) / 2;
			}
		}

		m_position = m_position + (m_velocity * (float)t.asSeconds());

		m_orientation = (atan2(m_velocity.y, m_velocity.x) * 180 / 3.14159265);

		m_sprite.GetSprite()->setPosition(m_position);
		m_sprite.Update();
	}

	if (m_health <= 0 && m_alive)
	{
		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Player")
			{
				m_entity->at(i)->m_score += m_countWorkers;
			}
		}
		m_explosionTimer = 120;
		m_alive = false;
	}
	else if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosion->Position(m_position);
		m_explosionTimer--;
	}
	else if (m_explosionTimer <= 0 && !m_alive)
	{
		m_position.x = -1000;
		m_position.y = -1000;
		m_sprite.GetSprite()->setPosition(m_position);
	}
}

void Sweeper::Draw(sf::RenderWindow & r)
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

//!Pursue Worker
/*! Pursues the worker nearby.*/
void Sweeper::PursueWorker(sf::Vector2f otherLocation, sf::Time t)
{
	float timetotarget = 0.01;

	float targetSpeed = 0;
	sf::Vector2f targetVelocity = sf::Vector2f(0, 0);

	if (GetMagnitude(otherLocation - m_position) < m_radiusStop)
	{
		targetSpeed = 0;
	}
	else if (GetMagnitude(otherLocation - m_position) > m_radiusSlow)
	{
		targetSpeed = m_maxspeed;
	}
	else
	{
		targetSpeed = m_maxspeed * (GetMagnitude(otherLocation - m_position) / m_radiusSlow);
	}

	targetVelocity = otherLocation - m_position;
	targetVelocity.x = targetVelocity.x / GetMagnitude(targetVelocity);
	targetVelocity.y = targetVelocity.y / GetMagnitude(targetVelocity);

	targetVelocity = targetVelocity * targetSpeed;

	sf::Vector2f m_linear = targetVelocity - m_velocity;
	m_linear = m_linear / timetotarget;

	if (GetMagnitude(m_linear) > m_maxAccel)
	{
		m_linear.x = m_linear.x / GetMagnitude(m_linear);
		m_linear.y = m_linear.y / GetMagnitude(m_linear);

		m_linear = m_linear * m_maxAccel;
	}

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

//!Flee
/*! Flees from player.*/
void Sweeper::Flee(sf::Time t, sf::Vector2f otherLocation)
{
	float timetotarget = 0.01;

	float targetSpeed = 0;
	sf::Vector2f targetVelocity = sf::Vector2f(0, 0);

	if (GetMagnitude(otherLocation - m_position) < m_radiusStop)
	{
		targetSpeed = 0;
	}
	else if (GetMagnitude(otherLocation - m_position) > m_radiusSlow)
	{
		targetSpeed = m_maxspeed;
	}
	else
	{
		targetSpeed = m_maxspeed * (GetMagnitude(otherLocation - m_position) / m_radiusSlow);
	}

	targetVelocity = otherLocation - m_position;
	targetVelocity.x = targetVelocity.x / GetMagnitude(targetVelocity);
	targetVelocity.y = targetVelocity.y / GetMagnitude(targetVelocity);

	targetVelocity = targetVelocity * targetSpeed;

	sf::Vector2f m_linear = targetVelocity - m_velocity;
	m_linear = m_linear / timetotarget;

	if (GetMagnitude(m_linear) > m_maxAccel)
	{
		m_linear.x = m_linear.x / GetMagnitude(m_linear);
		m_linear.y = m_linear.y / GetMagnitude(m_linear);

		m_linear = m_linear * m_maxAccel;
	}

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
	m_velocity.x = m_velocity.x * -1;
	m_velocity.y = m_velocity.y * -1;
}
