#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "stdafx.h"
//!Resource manager class
/*! deals with loading in the resources and assets and putting them into a vector*/
class ResourceManager
{
public:

	ResourceManager() {};
	~ResourceManager() {};

	void LoadTexture(std::string location, std::string id) 
	{
		sf::Texture * holder = new sf::Texture();
		if (!holder->loadFromFile(location))
		{
			std::cout << "ERROR: could not load " << id << std::endl;
		}
		else
		{
			m_textures.push_back(holder);
			m_textureId.push_back(id);
		}
	}

	sf::Texture * GetTexture(std::string id)
	{
		bool textureGet = false;
		for (int i = 0; i < m_textureId.size(); i++)
		{
			if (m_textureId.at(i) == id)
			{
				std::cout << id << std::endl;
				return m_textures.at(i);
				break;
			}
		}

		return 0;
	}

private:

	std::vector<sf::Texture *> m_textures;
	std::vector<std::string> m_textureId;

};
#endif