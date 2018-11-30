#include "ManagedSprite.h"

ManagedSprite::ManagedSprite(sf::Texture * t, int sizeX, int sizeY)
{
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*t);
	m_dimensions = sf::Vector2f(sizeX, sizeY);
	m_timer = 0;
	m_animate = false;
	m_state = 0;
}

void ManagedSprite::Update()
{
	//!Animate
	/*! If the sprite is animating it cycles through the animation cycle forward and backwards.*/
	if (m_animate)
	{
		if (m_timer <= 0)
		{
			if (m_forwardAnimate)
			{
				if (m_state != m_animStateEnd)
				{
					m_state++;
				}
				else
				{
					m_state--;
					m_forwardAnimate = false;
				}
			}
			else
			{
				if (m_state != m_animStateStart)
				{
					m_state--;
				}
				else
				{
					m_state++;
					m_forwardAnimate = true;
				}
			}
			m_timer = 10;
		}
		else
		{
			m_timer--;
		}
	}
	//!Texture Rect
	/*! Cuts and displays a sprite based on a given rectangle.*/
	m_sprite->setTextureRect(sf::IntRect(m_state * m_dimensions.x, 0, m_dimensions.x, m_dimensions.y));
}