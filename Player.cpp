#include "SFML/Graphics.hpp"
#include "Map.h"
#include "Point.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "Player.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Player::Player()
{
	character_type = pac::PLAYER;
	next_direction = pac::NONE;
	direction = pac::RIGHT;
	this->animation.time = pac::PLAYER_ANIMATION_TIME;
	move_speed = pac::PLAYER_MOVE_SPEED;
}

void pac::Player::chooseDirection(const sf::Event & event, sf::RenderWindow & window, pac::Map & map)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Right:
			setDirection(pac::RIGHT, map);
			break;
		case sf::Keyboard::Left:
			setDirection(pac::LEFT, map);
			break;
		case sf::Keyboard::Up:
			setDirection(pac::UP, map);
			break;
		case sf::Keyboard::Down:
			setDirection(pac::DOWN, map);
			break;
		}
	}
}

void pac::Player::setDirection(const pac::Direction & direction, pac::Map & map)
{
	if (!isColliding(direction, map))
	{
		move_without_checking = true;
		next_direction = pac::NONE;
		this->direction = direction;
	}
	else
	{
		if (velocity != 0)
		{
			next_direction = direction;
		}
	}
}

void pac::Player::eatPoints(pac::Map & map, pac::Blinky & blinky, pac::Pinky & pinky, pac::Inky & inky, pac::Clyde & clyde)
{
	if (map.getPoint(coordinate, direction, point))
	{
		if ((*point).getEatenState() == pac::NOT_EATEN)
		{
			if (direction == pac::RIGHT || direction == pac::LEFT)
			{
				if (lessThanOrEqual(getPosition().x + pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().x + pac::TILE_SIZE.width - (*point).getPointDistance()) && biggerThanOrEqual(getPosition().x + pac::CHARACTER_SIZE.width - pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().x + (*point).getPointDistance()) && lessThan(getPosition().y, (*point).getPosition().y) && biggerThan(getPosition().y + pac::CHARACTER_SIZE.height, (*point).getPosition().y + pac::TILE_SIZE.height))
				{
					(*point).eat();
					map.decrementPoints();
					if ((*point).getPointType() == pac::POWER_POINT)
					{
						frightenGhosts(blinky, pinky, inky, clyde);
					}
				}
			}
			else if (direction == pac::UP || direction == pac::DOWN)
			{
				if (lessThan(getPosition().x, (*point).getPosition().x) && biggerThan(getPosition().x + pac::CHARACTER_SIZE.width, (*point).getPosition().x + pac::TILE_SIZE.width) && lessThanOrEqual(getPosition().y + pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + pac::TILE_SIZE.height - (*point).getPointDistance()) && biggerThanOrEqual(getPosition().y + pac::CHARACTER_SIZE.height - pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + (*point).getPointDistance()))
				{
					(*point).eat();
					map.decrementPoints();
					if ((*point).getPointType() == pac::POWER_POINT)
					{
						frightenGhosts(blinky, pinky, inky, clyde);
					}
				}
			}
		}
	}
}

void pac::Player::frightenGhosts(pac::Blinky & blinky, pac::Pinky & pinky, pac::Inky & inky, pac::Clyde & clyde)
{
	blinky.frighten();
	pinky.frighten();
	inky.frighten();
	clyde.frighten();
}


void pac::Player::executeStoredDirection(pac::Map & map)
{
	if (next_direction != pac::NONE)
	{
		if (!isColliding(next_direction, map))
		{
			this->direction = next_direction;
			next_direction = pac::NONE;
			move_without_checking = true;
		}
	}
}

pac::Player::~Player()
{
}