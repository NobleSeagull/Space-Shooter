#include "Player.h"

Player::Player(ResourceManager * r, std::vector<Entity*> * e, Explosion * explosion, sf::View * cam, int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_explosion = explosion;

	m_entity = e;

	m_resource = r;

	m_sprite = ManagedSprite(r->GetTexture("player"), 16, 16);
	m_sprite.SetScale(2);

	m_sprite.SetIdle(0);
	m_sprite.SetAnimationStates(1, 2);

	m_sprite.GetSprite()->setOrigin(8, 8);

	m_speed = 0;

	m_type = "Player";

	m_cannon = new Cannon(r, e, explosion, x, y, "Player");

	m_health = 10;

	m_alive = true;

	m_fireRate = 25;
	m_cannon->FireRate(m_fireRate);

	m_camera = cam;

}

void Player::Update(sf::Time t, std::vector<Structure*>* s)
{
	if (m_alive)
	{
		m_cannon->FireRate(m_fireRate);
		Controls();

		m_velocity.x = m_speed * cos(m_orientation / (180 / 3.14159265));
		m_velocity.y = m_speed * sin(m_orientation / (180 / 3.14159265));

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
		for (int i = 0; i < m_entity->size(); i++)
		{
			if (m_entity->at(i)->Type() == "Worker")
			{
				if (DistanceFormula(m_entity->at(i)->Position(), m_position) < 32)
				{
					m_entity->at(i)->m_alive = false;
					m_entity->at(i)->m_health = 0;
					m_entity->at(i)->m_position.x = -1000;
					m_entity->at(i)->m_position.y = -1000;
					m_score++;
				}
			}
		}

		m_position.x = m_position.x + m_velocity.x * t.asSeconds();
		m_position.y = m_position.y + m_velocity.y * t.asSeconds();

		m_cannon->Position(m_position.x, m_position.y);
		m_cannon->Update(t, s);

		m_camera->setCenter(m_position.x, m_position.y);

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

void Player::Draw(sf::RenderWindow & r)
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

//!Controls
/*! Controls the player's inputs, applying them. Player moves like a tank.*/
void Player::Controls()
{
	bool isAccelerateOn = false;
	bool isButtonPressed = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		isAccelerateOn = true;
		isButtonPressed = true;
		m_speed += 2.5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (m_speed > 0)
		{
			m_speed -= 5;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_orientation -= 4;
		isButtonPressed = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_orientation += 4;
		isButtonPressed = true;
	}

	if (m_speed > 100)
	{
		m_speed = 100;
	}

	if (isButtonPressed)
	{
		m_sprite.AnimateOn();
	}
	else
	{
		m_sprite.AnimateOff();
	}

	if(!isAccelerateOn)
	{
		m_speed *= 0.95;
	}

	if (m_speed <= 0.4)
	{
		m_speed = 0;
	}
}