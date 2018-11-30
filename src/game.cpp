#include <SFML/Graphics.hpp>
#include <iostream>
#include "stdafx.h"

#include "ResourceManager.h"
#include "Player.h"
#include "Predator.h"
#include "Worker.h"
#include "Cannon.h"
#include "Space.h"
#include "Explosion.h"
#include "Graph.h"
#include "Missile.h"
#include "Nest.h"
#include "Radar.h"
#include "Block.h"
#include "Sweeper.h"
#include "PowerUp.h"

//! main cpp
/*! deals with the game loop and sfml windows loads all the textures and creates the astar grid with nodes*/
int main()
{
	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(600, 600), "SFML works!");

	sf::View *mainCamera = new sf::View(sf::FloatRect(0, 0, 600, 600));
	mainCamera->setCenter(300, 300);
	mainCamera->zoom(2.5);
	sf::View hudCamera = sf::View(sf::FloatRect(0, 0, 600, 600));
	hudCamera.zoom(0.5);
	hudCamera.setCenter(150, 150);
	Graph * solidMap = new Graph();
	srand(time(NULL));

	ResourceManager Resources;
	Resources.LoadTexture(".\\resources\\Player.png", "player");
	Resources.LoadTexture(".\\resources\\Space1.png", "space1");
	Resources.LoadTexture(".\\resources\\Space2.png", "space2");
	Resources.LoadTexture(".\\resources\\Space3.png", "space3");
	Resources.LoadTexture(".\\resources\\Predator.png", "predator");
	Resources.LoadTexture(".\\resources\\Worker.png", "worker");
	Resources.LoadTexture(".\\resources\\Cannon.png", "cannon");
	Resources.LoadTexture(".\\resources\\Bullet.png", "bullet");
	Resources.LoadTexture(".\\resources\\Explosion.png", "explosion");
	Resources.LoadTexture(".\\resources\\Missile.png", "missile");
	Resources.LoadTexture(".\\resources\\Nest.png", "nest");
	Resources.LoadTexture(".\\resources\\radar_none.png", "radar");
	Resources.LoadTexture(".\\resources\\radar_n.png",  "radar_n");
	Resources.LoadTexture(".\\resources\\radar_s.png",  "radar_s");
	Resources.LoadTexture(".\\resources\\radar_e.png",  "radar_e");
	Resources.LoadTexture(".\\resources\\radar_w.png",  "radar_w");
	Resources.LoadTexture(".\\resources\\radar_ne.png", "radar_ne");
	Resources.LoadTexture(".\\resources\\radar_nw.png", "radar_nw");
	Resources.LoadTexture(".\\resources\\radar_se.png", "radar_se");
	Resources.LoadTexture(".\\resources\\radar_sw.png", "radar_sw");
	Resources.LoadTexture(".\\resources\\Border.png", "border");
	Resources.LoadTexture(".\\resources\\astroid_wall.png", "a_wall");
	Resources.LoadTexture(".\\resources\\astroid_floor.png", "a_floor");
	Resources.LoadTexture(".\\resources\\Winner.png", "win");
	Resources.LoadTexture(".\\resources\\Power_Up.png", "powerup");

	ManagedSprite win(Resources.GetTexture("win"), 16, 16);
	win.SetScale(10);

	std::vector<Entity*> * entity = new std::vector<Entity*>();

	Explosion * explosion = new Explosion(&Resources, entity);
	explosion->m_position = sf::Vector2f(-1000, -1000);


	std::vector<ManagedSprite*> spaceTiles;
	spaceTiles.push_back(new ManagedSprite(Resources.GetTexture("space1"), 16, 16));
	spaceTiles.at(0)->SetScale(2);
	spaceTiles.at(0)->SetAnimationStates(0, 1);
	spaceTiles.at(0)->AnimateOn();

	spaceTiles.push_back(new ManagedSprite(Resources.GetTexture("space2"), 16, 16));
	spaceTiles.at(1)->SetScale(2);
	spaceTiles.at(1)->SetAnimationStates(0, 1);
	spaceTiles.at(1)->AnimateOn();

	spaceTiles.push_back(new ManagedSprite(Resources.GetTexture("space3"), 16, 16));
	spaceTiles.at(2)->SetScale(2);
	spaceTiles.at(2)->SetAnimationStates(0, 1);
	spaceTiles.at(2)->AnimateOn();

	ManagedSprite * border = new ManagedSprite(Resources.GetTexture("border"), 16, 16);
	border->SetScale(2);
	border->SetAnimationStates(0, 1);
	border->AnimateOn();

	ManagedSprite * astroidWall = new ManagedSprite(Resources.GetTexture("a_wall"), 16, 16);
	astroidWall->SetScale(2);

	ManagedSprite * astroidFloor = new ManagedSprite(Resources.GetTexture("a_floor"), 16, 16);
	astroidFloor->SetScale(2);

	std::vector<std::vector<std::string>> map =
	{ {"W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W",},
	{ "W","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","W", },
	{ "W","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","W", },
	{ "W","S","S","F","F","F","F","F","X","X","X","X","X","X","X","X","X","X","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","X","F","F","X","X","X","X","X","X","X","X","X","X","X","X","F","F","X","F","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","F","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","F","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","F","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","F","F","F","X","F","F","F","F","F","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","F","F","F","X","F","F","X","X","X","X","X","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","F","F","F","X","F","F","F","F","X","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","X","F","F","F","F","X","F","F","F","F","X","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","X","F","F","F","F","X","X","X","X","X","X","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","X","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","X","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","X","F","F","X","F","F","F","F","X","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","X","F","F","F","F","X","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","X","F","F","F","F","X","F","F","F","F","F","F","F","F","F","F","S","S","W", },
	{ "W","S","S","F","F","F","X","X","X","X","X","X","X","X","X","X","X","X","X","X","X","X","S","S","W", },
	{ "W","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","W", },
	{ "W","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","S","W", },
	{ "W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W","W", } };



	int width = 25;
	int height = 25;

	std::vector<Structure*> solidObjects;
	std::vector<std::vector<Structure*>> world;
	for (int i = 0; i < width; i++)
	{
		world.push_back(std::vector<Structure*>());
		std::cout << "" << std::endl;
		for (int j = 0; j < height; j++)
		{
			if (map.at(i).at(j) == "W")
			{
				world.at(i).push_back(new Block(border, i, j, true));
				solidMap->AddNode(i, j, true);
			}
			else if (map.at(i).at(j) == "S")
			{
				world.at(i).push_back(new Space(&spaceTiles, i, j));
				solidMap->AddNode(i, j, false);
			}
			else if (map.at(i).at(j) == "X")
			{
				world.at(i).push_back(new Block(astroidWall, i, j, true));
				solidMap->AddNode(i, j, true);
			}
			else if (map.at(i).at(j) == "F")
			{
				world.at(i).push_back(new Block(astroidFloor, i, j, false));
				solidMap->AddNode(i, j, false);
			}
		}
	}
	for (int i = 0; i < width; i++)
	{
		world.push_back(std::vector<Structure*>());
		for (int j = 0; j < height; j++)
		{
			if (world.at(i).at(j)->m_solid)
			{
				solidObjects.push_back(world.at(i).at(j));
			}
		}
	}

	solidMap->GenerateGraph();

	entity->push_back(new Player(&Resources, entity, explosion, mainCamera, -1000, -1000));
	entity->push_back(new Nest(&Resources, entity, explosion, solidMap, 4 * 32, 4 * 32));
	entity->push_back(new Nest(&Resources, entity, explosion, solidMap, 8 * 32, 10 * 32));
	//entity->push_back(new Nest(&Resources, entity, explosion, solidMap, 12 * 32, 8 * 32));
	entity->push_back(new Nest(&Resources, entity, explosion, solidMap, 16 * 32, 20 * 32));
	entity->push_back(new Nest(&Resources, entity, explosion, solidMap, 5 * 32, 20 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 2 * 32, 9 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 2 * 32, 5 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 2 * 32, 3 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 2 * 32, 3 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 6 * 32, 23 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 7 * 32, 3 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 8 * 32, 3 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 9 * 32, 6 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 10 * 32, 5 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 11 * 32, 6 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 12 * 32, 5 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 20 * 32, 10 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 8 * 32, 3 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 3 * 32, 8 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 10 * 32, 5 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 11 * 32, 6 * 32));
	entity->push_back(new Worker(&Resources, entity, solidMap, 12 * 32, 5 * 32));
	entity->push_back(new Sweeper(&Resources, entity, explosion, solidMap, 2.5 * 32, 10 * 32));
	entity->push_back(new PowerUp(&Resources, entity, 18 * 32, 10 * 32));
	entity->push_back(new PowerUp(&Resources, entity, 12 * 32, 4 * 32));
	entity->push_back(new PowerUp(&Resources, entity, 21 * 32, 21 * 32));
	entity->push_back(new PowerUp(&Resources, entity, 4 * 32, 4 * 32));
	entity->push_back(new PowerUp(&Resources, entity, 12 * 32, 12 * 32));

	Radar radar(&Resources, entity, 300-32,0);

	solidMap->Path(0, 0, 10, 10);
	entity->at(0)->m_position = sf::Vector2f(20*32, 10 * 32);

	//FPS stuff
	sf::Clock clock;
	const float FPS = 60.0f;
	const sf::Time timePerFrame = sf::seconds(1.0f / FPS);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	clock.restart();
	//

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		
		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > timePerFrame)
		{
			//Logic Code Goes Here
			explosion->Position(sf::Vector2f(-1000, -1000));
			explosion->Update(timeSinceLastUpdate, &solidObjects);
			for (int i = 0; i < spaceTiles.size(); i++)
			{
				spaceTiles.at(i)->Update();
			}
			border->Update();
			if (entity->at(0)->m_score < 10)
			{
				for (int i = 0; i < entity->size(); i++)
				{
					entity->at(i)->Update(timeSinceLastUpdate, &solidObjects);
				}
			}
			radar.Update();
			//No More

			window->clear();
			//Display Code Goes Here
			for (int i = 0; i < world.size(); i++)
			{
				for (int j = 0; j < world.at(i).size(); j++)
				{
					world.at(i).at(j)->Draw(*window);
				}
			}
			for (int i = 0; i < entity->size(); i++)
			{
				entity->at(i)->Draw(*window);
			}
			window->setView(hudCamera);
			radar.Draw(*window);
			if (entity->at(0)->m_score >= 10)
			{
				window->draw(*win.GetSprite());
			}
			//No More
			window->display();
			window->setView(*mainCamera);

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	return 0;
}