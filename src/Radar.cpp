#include "Radar.h"

Radar::Radar(ResourceManager * r, std::vector<Entity*> * e, int x, int y)
{
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_n"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_s"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_e"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_w"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_ne"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_nw"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_se"), 16, 16));
	m_sprites.push_back(ManagedSprite(r->GetTexture("radar_sw"), 16, 16));

	for (int i = 0; i < m_sprites.size(); i++)
	{
		m_sprites.at(i).SetScale(2);
	}

	m_position.x = x;
	m_position.y = y;

	m_resource = r;
	m_entity = e;
}


//!Update Radar
/*! Tracks all civilian ships. Points to them on it's radar.*/
void Radar::Update()
{
	n =  false;
	s =  false;
	e =  false;
	w =  false;
	ne = false;
	nw = false;
	se = false;
	sw = false;

	int playerId = 0;

	for (int i = 0; i < m_entity->size(); i++)
	{
		if (m_entity->at(i)->Type() == "Player")
		{
			playerId = i;
		}
	}

	for (int i = 0; i < m_entity->size(); i++)
	{
		if (m_entity->at(i)->Type() == "Worker" && m_entity->at(i)->m_alive || m_entity->at(i)->Type() == "Worker" && m_entity->at(i)->m_health > 0 || m_entity->at(i)->Type() == "Worker" && m_entity->at(i)->m_position.x >= -800)
		{
			int holder = atan2(m_entity->at(i)->Position().x - m_entity->at(playerId)->Position().x, m_entity->at(i)->Position().y - m_entity->at(playerId)->Position().y) * 180 / 3.14;
			if (holder > 0)
			{
				int temp = holder / 360;
				holder = holder - (temp * 360);
			}
			else if (holder != 0)
			{
				int temp = holder / 360;
				temp -= 1;
				holder = holder - (temp * 360);
			}

			if (holder >= 0 && holder <= 21)
			{
				s = true;
			}
			else if (holder >= 22 && holder <= 67)
			{
				se = true;
			}
			else if (holder >= 68 && holder <= 112)
			{
				e = true;
			}
			else if (holder >= 113 && holder <= 158)
			{
				ne = true;
			}
			else if (holder >= 157 && holder <= 202)
			{
				n = true;
			}
			else if (holder >= 203 && holder <= 248)
			{
				nw = true;
			}
			else if (holder >= 249 && holder <= 292)
			{
				w = true;
			}
			else if (holder >= 293 && holder <= 337)
			{
				sw = true;
			}
			else
			{
				s = true;
			}
		}

	}
	for (int i = 0; i < m_sprites.size(); i++)
	{
		m_sprites.at(i).GetSprite()->setPosition(m_position);
		m_sprites.at(i).Update();
	}
}

void Radar::Draw(sf::RenderWindow & r)
{
	r.draw(*m_sprites.at(0).GetSprite());
	if (n)
	{
		r.draw(*m_sprites.at(1).GetSprite());
	}
	if (s)
	{
		r.draw(*m_sprites.at(2).GetSprite());
	}
	if (e)
	{
		r.draw(*m_sprites.at(3).GetSprite());
	}
	if (w)
	{
		r.draw(*m_sprites.at(4).GetSprite());
	}
	if (ne)
	{
		r.draw(*m_sprites.at(5).GetSprite());
	}
	if (nw)
	{
		r.draw(*m_sprites.at(6).GetSprite());
	}
	if (se)
	{
		r.draw(*m_sprites.at(7).GetSprite());
	}
	if (sw)
	{
		r.draw(*m_sprites.at(8).GetSprite());
	}
}