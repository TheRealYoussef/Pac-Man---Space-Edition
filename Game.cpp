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
	map = new pac::Map;
	(*map).init(pac::Position( 144.f, 52.f ), "Assets/Maps/Pac Man Map.txt");
	player = new pac::Player;
	(*player).init((*map).getPlayerStartingPosition(), "Assets/Sprites/Pac Man.png", *map);
	blinky = new pac::Blinky;
	(*blinky).init((*map).getBlinkyStartingPosition(), "Assets/Sprites/Blinky.png", *map);
	(*blinky).setScatterPosition(*map, pac::Coordinate((*map).getMapSize().col, 0));
	pinky = new pac::Pinky;
	(*pinky).init((*map).getPinkyStartingPosition(), "Assets/Sprites/Pinky.png", *map);
	(*pinky).setScatterPosition(*map, pac::Coordinate(0, 0));
	inky = new pac::Inky;
	(*inky).init((*map).getInkyStartingPosition(), "Assets/Sprites/Inky.png", *map);
	(*inky).setScatterPosition(*map, pac::Coordinate((*map).getMapSize().col, (*map).getMapSize().row));
	clyde = new pac::Clyde;
	(*clyde).init((*map).getClydeStartingPosition(), "Assets/Sprites/Clyde.png", *map);
	(*clyde).setScatterPosition(*map, pac::Coordinate(0, (*map).getMapSize().row));
}

void pac::Game::functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window)
{
	(*player).chooseDirection(sf_event, window, *map);
}

void pac::Game::functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame)
{
	(*blinky).changeMode(*map);
	(*blinky).playAnimation();
	(*blinky).targetPosition(*player);
	(*blinky).chooseBestDirection(*map);
	(*blinky).move(*map, time_per_frame);
	(*pinky).changeMode(*map);
	(*pinky).playAnimation();
	(*pinky).targetPosition(*player);
	(*pinky).chooseBestDirection(*map);
	(*pinky).move(*map, time_per_frame);
	(*inky).changeMode(*map);
	(*inky).playAnimation();
	(*inky).targetPosition(*player, *blinky);
	(*inky).chooseBestDirection(*map);
	(*inky).move(*map, time_per_frame);
	(*clyde).changeMode(*map);
	(*clyde).playAnimation();
	(*clyde).targetPosition(*player, *map);
	(*clyde).chooseBestDirection(*map);
	(*clyde).move(*map, time_per_frame);
	(*player).playAnimation();
	(*player).executeStoredDirection(*map);
	(*player).move(*map, time_per_frame);
	(*player).eatPoints(*map, *blinky, *pinky, *inky, *clyde);
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