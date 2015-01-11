#include "SFML/Graphics.hpp"

#include "Game.h"
#include "ButtonArray.h"
#include "Map.h"
#include "Player.h"
#include "GlobalVariables.h"

pac::Game::Game()
{
	map.init(pac::Position{ 144.f, 52.0f }, "Assets/Maps/Pac Man Map.txt");
	player.init(map.getPlayerStartingPosition(), 250.0f, "Assets/Sprites/Pac Man.png", map);
}

void pac::Game::functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window, const float & time_per_frame)
{
	player.chooseDirection(sf_event, window, time_per_frame, map);
}

void pac::Game::functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame)
{
	player.playAnimation();
	player.executeStoredDirection(time_per_frame, map);
	player.move(map, time_per_frame);
	player.eatPoints(map);
}

void pac::Game::display(sf::RenderWindow & window)
{
	map.display(window);
	map.displayPoints(window);
	player.display(window);
	map.displayTeleportationTiles(window);
}

pac::Game::~Game()
{
}