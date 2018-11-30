#ifndef SPACE_H
#define SPACE_H

#include "Structure.h"
//!Space class
/*! this creates the randomly generated background to make the world look unique*/
class Space : public Structure
{
public:
	Space(std::vector<ManagedSprite*>* sprite, int x, int y);
	~Space() {};

	void Update(sf::Time t) {}
	void Draw(sf::RenderWindow & r);

	bool Solid() { return m_solid; };

private:

	std::vector<ManagedSprite*>* m_sprite;
	int m_spaceTile = 0;

};
#endif