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
	map.init(pac::Position( 144.f, 52.f ), "Assets/Maps/Pac Man Map.txt");
	player.init(map.getPlayerStartingPosition(), "Assets/Sprites/Pac Man.png", map);
	blinky.init(pac::Position(458.f, 220.f), "Assets/Sprites/Blinky.png", map); 
	blinky.setScatterPosition(map, pac::Coordinate(map.getMapSize().col, 0));
	pinky.init(pac::Position(326.f, 220.f), "Assets/Sprites/Pinky.png", map); 
	pinky.setScatterPosition(map, pac::Coordinate(0, 0));
	inky.init(pac::Position(442.f, 220.f), "Assets/Sprites/Inky.png", map); 
	inky.setScatterPosition(map, pac::Coordinate(map.getMapSize().col, map.getMapSize().row));
	clyde.init(pac::Position(384.f, 316.f), "Assets/Sprites/Clyde.png", map); 
	clyde.setScatterPosition(map, pac::Coordinate(0, map.getMapSize().row));
}

void pac::Game::functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window)
{
	player.chooseDirection(sf_event, window, map);
}

void pac::Game::functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame)
{
	blinky.changeMode();
	blinky.playAnimation();
	blinky.targetPlayer(player);
	blinky.chooseBestDirection(map);
	blinky.move(map, time_per_frame);
	pinky.changeMode();
	pinky.playAnimation();
	pinky.targetInfrontOfPlayer(player);
	pinky.chooseBestDirection(map);
	pinky.move(map, time_per_frame);
	inky.changeMode();
	inky.playAnimation();
	inky.targetDoubleBlinkyPlayerDistance(player, blinky);
	inky.chooseBestDirection(map);
	inky.move(map, time_per_frame);
	clyde.changeMode();
	clyde.playAnimation();
	clyde.targetPlayerOrScatterTile(player, map);
	clyde.chooseBestDirection(map);
	clyde.move(map, time_per_frame);
	player.playAnimation();
	player.executeStoredDirection(map);
	player.move(map, time_per_frame);
	player.eatPoints(map);
}

void pac::Game::display(sf::RenderWindow & window)
{
	map.display(window);
	map.displayPoints(window);
	player.display(window);
	blinky.display(window);
	pinky.display(window);
	inky.display(window);
	clyde.display(window);
	map.displayTeleportationTiles(window);
}

pac::Game::~Game()
{
}