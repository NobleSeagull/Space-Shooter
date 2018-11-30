#include "Predator.h"

Predator::Predator(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, Graph * g, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_graph = g;
	m_entity = e;

	m_explosion = explosion;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("predator"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.GetSprite()->setOrigin(8, 8);
	m_sprite.SetAnimationStates(0, 1);
	m_sprite.AnimateOn();

	m_isSlave = false;
	m_leader = false;

	m_type = "Predator";

	m_pathRenewTimer = 0;

	m_alive = true;

	m_cannon = new Cannon(r, e, explosion, x, y, "Enemy");
	m_cannon->FireRate(50);
	m_cannon->BulletSpeed(25);

	m_health = 2;

	m_speed = 0;
}

//!Predator Update
/*! Predator Update is the largest and most complicated function.*/
void Predator::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		//!Update Path and Get Slaves
		/*! Predator hunts down the player using ASTAR, when within a certain distance, they use pursue upon the player. If it comes in contact with other predators, it will be either given a leader or get a slave. Form a + around the player when given the chance.*/
		if (m_pathRenewTimer <= 0 && !m_isSlave)
		{
			for (int i = 0; i < m_entity->size(); i++)
			{
				if (m_entity->at(i)->Type() == "Player" && m_entity->at(i) != this)
				{
					if (DistanceFormula(m_entity->at(i)->Position(), m_position) > 32 * 5)
					{
						m_path = m_graph->Path((int)(m_entity->at(i)->Position().x / 32), (int)(m_entity->at(i)->Position().y / 32), (int)(m_position.x / 32), (int)(m_position.y / 32));
					}
				}
				else if (m_entity->at(i)->Type() == "Predator" && m_entity->at(i) != this)
				{
					if (DistanceFormula(m_entity->at(i)->Position(), m_position) < 160)
					{
						if (!m_entity->at(i)->m_isSlave && !m_entity->at(i)->m_leader && m_formation.size() < 3)
						{
							m_leader = true;
							m_formation.push_back(m_entity->at(i));
							m_formation.back()->m_isSlave = true;
							m_formation.back()->m_master = this;
							std::cout << "I have enslaved " << i << "!" << std::endl;
						}
						else if (m_entity->at(i)->m_leader && m_entity->at(i)->m_formation.size() <= m_formation.size() && m_formation.size() < 3)
						{
							m_leader = true;
							m_formation.push_back(m_entity->at(i)->m_formation.back());
							m_entity->at(i)->m_formation.pop_back();
							m_formation.back()->m_isSlave = true;
							m_formation.back()->m_master = this;
							std::cout << "I have stolen a slave from " << i << "!" << std::endl;
						}
					}
				}
				m_pathRenewTimer = 150;
			}
		}
		else if (!m_isSlave)
		{
			m_pathRenewTimer--;
		}
		//!Formation
		/*! Achieves formation as given above. Only allows 4 to be in a squad. Creates a + shape, to surround the player.*/
		int playerIndex = -1;
		bool attackPlayer = false;
		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Player")
			{
				playerIndex = i;
				if (DistanceFormula(m_entity->at(i)->Position(), m_position) < 32 * 3)
				{
					attackPlayer = true;
					break;
				}
			}
		}
		if (m_leader)
		{
			if (m_formation.size() <= 0)
			{
				m_leader = false;
			}
			else if (attackPlayer)
			{
				m_path.clear();
				m_path.push_back(Node((m_entity->at(playerIndex)->Position().x / 32) - 2, (m_entity->at(playerIndex)->Position().y / 32), false));
				bool slaveOne = false;
				bool slaveTwo = false;
				for (int i = 0; i < m_formation.size(); i++)
				{
					m_formation.at(i)->m_path.clear();
					if (!slaveOne)
					{
						m_formation.at(i)->m_path.push_back(Node((m_entity->at(playerIndex)->Position().x / 32) + 2, m_entity->at(playerIndex)->Position().y / 32, false));
						slaveOne = true;
					}
					else if (!slaveTwo)
					{
						m_formation.at(i)->m_path.push_back(Node((m_entity->at(playerIndex)->Position().x / 32), (m_entity->at(playerIndex)->Position().y / 32) + 2, false));
						slaveTwo = true;
					}
					else
					{
						m_formation.at(i)->m_path.push_back(Node((m_entity->at(playerIndex)->Position().x / 32), (m_entity->at(playerIndex)->Position().y / 32) - 2, false));
					}
				}
			}
			else
			{
				for (int i = 0; i < m_formation.size(); i++)
				{
					m_formation.at(i)->m_path.push_back(Node((m_position.x / 32), (m_position.y / 32), false));
				}
			}
		}
		//
		if (attackPlayer)
		{
			float dx, dy;
			dx = m_entity->at(playerIndex)->Position().x - (m_position.x);
			dy = m_entity->at(playerIndex)->Position().y - (m_position.y);
			float tempOrientation = (atan2(dy, dx)) * 180 / 3.14;
			m_cannon->Orientation(tempOrientation);
			m_cannon->ForceFire();
		}

		//!Pop Path as they cross points
		/*! Every point they cross is popped from their path, in which they progress forward.*/
		if (m_path.size() > 0)
		{
			sf::Rect<int> holderThis = sf::Rect<int>(m_position.x - 16, m_position.y - 16, 32, 32);
			sf::Rect<int> holderOther = sf::Rect<int>((m_path.back().x * 32), (m_path.back().y * 32), 16, 16);

			if (holderThis.intersects(holderOther))
			{
				if (m_path.size() > 1)
				{
					m_path.pop_back();
				}
			}
			Pursue(sf::Vector2f((m_path.back().x * 32)+16, (m_path.back().y * 32)+16), t);
		}

		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i) != this && m_entity->at(i)->Type() != "bullet")
			{
				sf::Vector2f tempPos = sf::Vector2f(m_position.x - 16, m_position.y - 16);
				tempPos.x = tempPos.x + (m_velocity.x * (float)t.asSeconds());
				sf::Rect<int> tempRect1 = sf::Rect<int>(tempPos.x, tempPos.y, 32, 32);
				sf::Rect<int> tempRect2 = sf::Rect<int>(m_entity->at(i)->Position().x - 16, m_entity->at(i)->Position().y - 16, 32, 32);

				sf::Vector2f tempPos2 = sf::Vector2f(m_position.x - 16, m_position.y - 16);
				tempPos2.y = tempPos2.y + (m_velocity.y * (float)t.asSeconds());
				sf::Rect<int> tempRect3 = sf::Rect<int>(tempPos2.x, tempPos2.y, 32, 32);
				sf::Rect<int> tempRect4 = sf::Rect<int>(m_entity->at(i)->Position().x - 16, m_entity->at(i)->Position().y - 16, 32, 32);
				if (tempRect1.intersects(tempRect2) && m_entity->at(i)->Type() != "Missile" && m_entity->at(i)->Type() != "Nest")
				{
					m_velocity.x = (m_velocity.x * -1) / 2;
					m_entity->at(i)->m_velocity.x = (m_velocity.x / 2) * -1;

				}
				else if (tempRect3.intersects(tempRect4) && m_entity->at(i)->Type() != "Missile" && m_entity->at(i)->Type() != "Nest")
				{
					m_velocity.y = (m_velocity.y * -1) / 2;
					m_entity->at(i)->m_velocity.y = (m_velocity.y / 2) * -1;

				}
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
				m_velocity.x = (m_velocity.x * -1) / 2;

			}
			else if (tempRect3.intersects(tempRect4))
			{
				m_velocity.y = (m_velocity.y * -1) / 2;
			}

		}

		m_position = m_position + (m_velocity * (float)t.asSeconds());

		m_orientation = (atan2(m_velocity.y, m_velocity.x) * 180 / 3.14159265);

		m_cannon->Position(m_position.x, m_position.y);
		m_cannon->Update(t, s);

		m_sprite.GetSprite()->setPosition(m_position);
		m_sprite.Update();
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
}

void Predator::Draw(sf::RenderWindow & r)
{
	if (m_alive)
	{
		m_sprite.GetSprite()->setRotation(m_orientation);
		r.draw(*m_sprite.GetSprite());
		m_cannon->Draw(r);
	}
	if (m_explosionTimer > 0 && !m_alive)
	{
		m_explosion->m_position = m_position;
		m_explosion->Draw(r);
	}
}

//!Pursue
/*! Pursues a point, slowing as it gets close. Used for more accurate movements.*/
void Predator::Pursue(sf::Vector2f otherLoc, sf::Time t)
{
	float timetotarget = 0.05;

	float targetSpeed = 0;
	sf::Vector2f targetVelocity = sf::Vector2f(0, 0);

	if (GetMagnitude(otherLoc - m_position) < m_radiusStop)
	{
		targetSpeed = 0;
	}
	else if (GetMagnitude(otherLoc - m_position) > m_radiusSlow)
	{
		targetSpeed = m_maxspeed;
	}
	else
	{
		targetSpeed = m_maxspeed * (GetMagnitude(otherLoc - m_position) / m_radiusSlow);
	}

	targetVelocity = otherLoc - m_position;
	targetVelocity.x = targetVelocity.x / GetMagnitude(targetVelocity);
	targetVelocity.y = targetVelocity.y / GetMagnitude(targetVelocity);

	targetVelocity = targetVelocity * targetSpeed;

	sf::Vector2f m_linear = targetVelocity - m_velocity;
	m_linear = m_linear / timetotarget;

	if (GetMagnitude(m_linear) > m_maxaccel)
	{
		m_linear.x = m_linear.x / GetMagnitude(m_linear);
		m_linear.y = m_linear.y / GetMagnitude(m_linear);

		m_linear = m_linear * m_maxaccel;
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
