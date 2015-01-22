#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include "Enemy.h"
#include "Map.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Enemy::Enemy()
{
	animation.time = pac::GHOST_ANIMATION_TIME;
	mode = pac::SCATTER;
	forced_switch = false;
	previous_directions.resize(pac::NUMBER_OF_SAFETY_FRAMES);
	for (int i = 0; i < pac::NUMBER_OF_SAFETY_FRAMES; i++)
	{
		previous_directions[i] = direction;
	}
	move_speed = pac::ENEMY_MOVE_SPEED;
	scatter_clock.restart();
}

void pac::Enemy::setScatterPosition(const pac::Map & map, const pac::Coordinate & coordinate)
{
	scatter_position = pac::Position(map.getMapPosition().x + coordinate.x * pac::TILE_SIZE.width + pac::TILE_SIZE.width / 2.f, map.getMapPosition().y + coordinate.y * pac::TILE_SIZE.height + pac::TILE_SIZE.height / 2.f);
}

void pac::Enemy::chooseBestDirection(pac::Map & map)
{
	distances.resize(0);

	center_coordinate = map.getCoordinate(pac::Position( getPosition().x + pac::CHARACTER_SIZE.width / 2.f, getPosition().y + pac::CHARACTER_SIZE.height / 2.f ));

	calculateDistance(pac::Coordinate(center_coordinate.x + 1, center_coordinate.y), pac::RIGHT, map);
	calculateDistance(pac::Coordinate(center_coordinate.x - 1, center_coordinate.y), pac::LEFT, map);
	calculateDistance(pac::Coordinate(center_coordinate.x, center_coordinate.y - 1), pac::UP, map);
	calculateDistance(pac::Coordinate(center_coordinate.x, center_coordinate.y + 1), pac::DOWN, map);

	same_direction = true;

	for (int i = 0; i < NUMBER_OF_SAFETY_FRAMES - 1; i++)
	{
		previous_directions[i] = previous_directions[i + 1];
	}

	previous_directions[NUMBER_OF_SAFETY_FRAMES - 1] = direction;

	compareWithCurrentDirection(false);

	compareWithAnotherDirection(map);

}

void pac::Enemy::calculateDistance(const pac::Coordinate & coordinate, const pac::Direction & direction, pac::Map & map)
{
	if (!isColliding(direction, map) && checkPreviousDirectionsOpposites(direction))
	{
		distances.push_back(sqrt(pow(target_position.x - (map.getPosition(coordinate).x + pac::TILE_SIZE.width / 2.0), 2) + pow(target_position.y - (map.getPosition(coordinate).y + pac::TILE_SIZE.height / 2.0), 2)));
	}
	else
	{
		distances.push_back(-1.f);
	}
}

void pac::Enemy::compareWithCurrentDirection(const bool & same_direction)
{
	for (int i = 0; i < pac::NUMBER_OF_POSSIBLE_DIRECTIONS; i++)
	{
		if (distances[i] != -1.f)
		{
			if (checkPreviousDirectionsOpposites((pac::Direction)i) && checkPreviousDirections((pac::Direction)i))
			{
				if (distances[i] < distances[direction])
				{
					direction = (pac::Direction)i;
					this->same_direction = same_direction;
				}
			}
		}
	}
}

void pac::Enemy::compareWithAnotherDirection(Map & map)
{
	if (same_direction)
	{
		if (isColliding(previous_directions[NUMBER_OF_SAFETY_FRAMES - 1], map))
		{
			getOtherDirection();
			compareWithCurrentDirection(true);
		}
	}
}

void pac::Enemy::getOtherDirection()
{
	for (int i = 0; i < pac::NUMBER_OF_POSSIBLE_DIRECTIONS; i++)
	{
		if (distances[i] != -1.f)
		{
			if (checkPreviousDirectionsOpposites((pac::Direction)i) && checkPreviousDirections((pac::Direction)i))
			{
				direction = (pac::Direction)i;
				break;
			}
		}
	}
}

void pac::Enemy::changeMode()
{
	switch (mode)
	{
	case pac::CHASE:
		if (chase_clock.getElapsedTime() >= pac::CHASE_TIME)
		{
			mode = pac::SCATTER;
			forced_switch = false;
			scatter_clock.restart();
		}
		break;
	case pac::SCATTER:
		if (!forced_switch)
		{
			for (int i = 0; i < NUMBER_OF_SAFETY_FRAMES - 1; i++)
			{
				previous_directions[i] = previous_directions[i + 1];
			}
			previous_directions[NUMBER_OF_SAFETY_FRAMES - 1] = direction;
			direction = opposite(direction);
			forced_switch = true;
		}
		target_position = scatter_position;
		if (scatter_clock.getElapsedTime() >= pac::SCATTER_TIME)
		{
			mode = pac::CHASE;
			chase_clock.restart();
		}
		break;
	case pac::FRIGHTENED:
		if (frightened_clock.getElapsedTime() >= pac::FRIGHTENED_TIME)
		{
			mode = pac::CHASE;
			chase_clock.restart();
		}
		break;
	}
}

bool pac::Enemy::checkPreviousDirections(const pac::Direction & direction)
{
	for (int i = 0; i < NUMBER_OF_SAFETY_FRAMES; i++)
	{
		if (direction == previous_directions[i])
		{
			return false;
		}
	}
	return true;
}

bool pac::Enemy::checkPreviousDirectionsOpposites(const pac::Direction & direction)
{
	for (int i = 0; i < NUMBER_OF_SAFETY_FRAMES; i++)
	{
		if (direction == opposite(previous_directions[i]))
		{
			return false;
		}
	}
	return true;
}