#include "SFML/Graphics.hpp"

#include <cmath>

#include "Game.h"
#include "ButtonArray.h"
#include "Map.h"
#include "Player.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "GlobalVariables.h"

pac::Game::Game()
{
	loadMap();
	
	loadPlayer();

	loadBlinky();

	loadPinky();

	loadInky();
	
	loadClyde();

	restartClocks();
}

void pac::Game::loadMap()
{
	map = new pac::Map;
	(*map).init(pac::MAP_POSITION, "Assets/Maps/Pac Man Map.txt");
}

void pac::Game::loadPlayer()
{
	player = new pac::Player;
	(*player).init((*map).getPlayerStartingPosition(), "Assets/Sprites/Pac Man.png", *map);
}

void pac::Game::loadBlinky()
{
	blinky = new pac::Blinky;
	(*blinky).init((*map).getBlinkyStartingPosition(), "Assets/Sprites/Blinky.png", *map);
	(*blinky).setScatterPosition(*map, pac::Coordinate((*map).getMapSize().col, 0));
	(*blinky).loadImages("Assets/Sprites/Frightened 1.png", "Assets/Sprites/Frightened 2.png", "Assets/Sprites/Blinky.png");
}

void pac::Game::loadPinky()
{
	pinky = new pac::Pinky;
	(*pinky).init((*map).getPinkyStartingPosition(), "Assets/Sprites/Pinky.png", *map);
	(*pinky).loadImages("Assets/Sprites/Frightened 1.png", "Assets/Sprites/Frightened 2.png", "Assets/Sprites/Blinky.png");
	(*pinky).setScatterPosition(*map, pac::Coordinate(0, 0));
}

void pac::Game::loadInky()
{
	inky = new pac::Inky;
	(*inky).init((*map).getInkyStartingPosition(), "Assets/Sprites/Inky.png", *map);
	(*inky).setScatterPosition(*map, pac::Coordinate((*map).getMapSize().col, (*map).getMapSize().row));
	(*inky).loadImages("Assets/Sprites/Frightened 1.png", "Assets/Sprites/Frightened 2.png", "Assets/Sprites/Blinky.png");
}

void pac::Game::loadClyde()
{
	clyde = new pac::Clyde;
	(*clyde).init((*map).getClydeStartingPosition(), "Assets/Sprites/Clyde.png", *map);
	(*clyde).setScatterPosition(*map, pac::Coordinate(0, (*map).getMapSize().row));
	(*clyde).loadImages("Assets/Sprites/Frightened 1.png", "Assets/Sprites/Frightened 2.png", "Assets/Sprites/Blinky.png");
}

void pac::Game::restartClocks()
{
	ghost_animation_clock.restart();
	player_animation_clock.restart();
	frightened_animation_clock.restart();
}

void pac::Game::functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window)
{
	(*player).chooseDirection(sf_event, window, *map);
}

void pac::Game::functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame)
{
	playerFunctions(time_per_frame);

	enemiesAnimations();

	enemiesFrightenedAnimations();

	enemyFunctions(blinky, time_per_frame);

	enemyFunctions(pinky, time_per_frame);

	inkyFunctions(time_per_frame);

	enemyFunctions(clyde, time_per_frame);
}

void pac::Game::playerFunctions(const float & time_per_frame)
{
	if (player_animation_clock.getElapsedTime() >= pac::PLAYER_ANIMATION_TIME)
	{
		(*player).playAnimation();
		player_animation_clock.restart();
	}
	(*player).executeStoredDirection(*map);
	(*player).eatPoints(*map, *blinky, *pinky, *inky, *clyde);
	(*player).move(*map, time_per_frame);
	(*player).teleport();
}

void pac::Game::enemiesAnimations()
{
	if (ghost_animation_clock.getElapsedTime() >= pac::GHOST_ANIMATION_TIME)
	{
		(*blinky).playAnimation();
		(*pinky).playAnimation();
		(*inky).playAnimation();
		(*clyde).playAnimation();
		ghost_animation_clock.restart();
	}
}

void pac::Game::enemiesFrightenedAnimations()
{
	if (frightened_animation_clock.getElapsedTime() >= pac::FRIGHTENED_ANIMATION_TIME)
	{
		(*blinky).frightenedAnimation();
		(*pinky).frightenedAnimation();
		(*inky).frightenedAnimation();
		(*clyde).frightenedAnimation();
		frightened_animation_clock.restart();
	}
}

void pac::Game::enemyFunctions(pac::Enemy *enemy, const float & time_per_frame)
{
	(*enemy).changeMode(*map);
	(*enemy).targetPosition(*player);
	(*enemy).chooseBestDirection(*map);
	(*enemy).move(*map, time_per_frame);
	(*enemy).moveFrightened();
	(*enemy).teleport();
}

void pac::Game::inkyFunctions(const float & time_per_frame)
{
	(*inky).changeMode(*map);
	(*inky).targetPosition(*player, *blinky);
	(*inky).chooseBestDirection(*map);
	(*inky).move(*map, time_per_frame);
	(*inky).moveFrightened();
	(*inky).teleport();
}

void pac::Game::display(sf::RenderWindow & window)
{
	(*map).display(window);
	(*player).display(window);
	(*blinky).display(window);
	(*pinky).display(window);
	(*inky).display(window);
	(*clyde).display(window);
	(*map).displayTeleportationTiles(window);
}

pac::Game::~Game()
{
	delete clyde;
	delete inky;
	delete pinky;
	delete blinky;
	delete player;
	delete map;
}