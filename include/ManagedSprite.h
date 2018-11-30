#ifndef MANAGEDSPRITE_H
#define MANAGEDSPRITE_H

#include "stdafx.h"
//! maneged sprite class
/*! Managed sprite is a derivative of the sfml sprite class, but manages animation and scale.*/
class ManagedSprite
{
public:
	ManagedSprite(sf::Texture * t, int sizeX, int sizeY);
	ManagedSprite() {};
	~ManagedSprite() {};

	void Update();

	sf::Sprite * GetSprite()
	{
		return m_sprite;
	}

	void AnimateOn()
	{
		if (!m_animate)
		{
			m_animate = true;
			m_state = m_animStateStart;
		}
	}

	void AnimateOff()
	{
		if (m_animate)
		{
			m_animate = false;
			m_state = m_idleState;
		}
	}

	void SetScale(int x)
	{
		m_sprite->setScale(sf::Vector2f(x, x));
	}

	void SetIdle(int x)
	{
		m_idleState = x;
	}
	void SetAnimationStates(int x, int y)
	{
		m_animStateStart = x;
		m_animStateEnd = y;
	}

private:

	sf::Sprite * m_sprite;
	sf::Vector2f m_dimensions;
	int m_idleState;
	int m_animStateStart, m_animStateEnd;
	int m_state;
	bool m_animate;
	bool m_forwardAnimate;
	int m_timer;
};
#endif

