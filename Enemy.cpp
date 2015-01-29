#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Enemy.h"
#include "Map.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Enemy::Enemy()
{
	character_type = pac::ENEMY;
	animation.time = pac::GHOST_ANIMATION_TIME;
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
	if (mode != pac::HOUSE && mode != pac::LEAVE_HOUSE)
	{
		distances.resize(0);

		center_coordinate = map.getCoordinate(pac::Position(getPosition().x + pac::CHARACTER_SIZE.width / 2.f, getPosition().y + pac::CHARACTER_SIZE.height / 2.f));

		calculateDistance(pac::Coordinate(center_coordinate.x + 1, center_coordinate.y), pac::RIGHT, map);
		calculateDistance(pac::Coordinate(center_coordinate.x - 1, center_coordinate.y), pac::LEFT, map);
		calculateDistance(pac::Coordinate(center_coordinate.x, center_coordinate.y - 1), pac::UP, map);
		calculateDistance(pac::Coordinate(center_coordinate.x, center_coordinate.y + 1), pac::DOWN, map);

		same_direction = true;

		updatePreviousDirections(direction);

		compareWithCurrentDirection(false);

		compareWithAnotherDirection(map);
	}
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

void pac::Enemy::changeMode(pac::Map & map)
{
	if (map.getEatenPoints() == get_out_of_ghost_house_points)
	{
		mode = pac::LEAVE_HOUSE;
	}
	switch (mode)
	{
	case pac::CHASE:
		chase();
		break;
	case pac::SCATTER:
		scatter();
		break;
	case pac::FRIGHTENED:
		run(map);
		break;
	case pac::HOUSE:
		house(map);
		break;
	case pac::LEAVE_HOUSE:
		leaveHouse(map);
		break;
	}
}

void pac::Enemy::chase()
{
	if (chase_clock.getElapsedTime() >= pac::CHASE_TIME)
	{
		mode = pac::SCATTER;
		forced_switch = true;
		scatter_clock.restart();
	}
}

void pac::Enemy::scatter()
{
	if (forced_switch)
	{
		updatePreviousDirections(direction);
		direction = opposite(direction);
		forced_switch = false;
	}
	target_position = scatter_position;
	if (scatter_clock.getElapsedTime() >= pac::SCATTER_TIME)
	{
		mode = pac::CHASE;
		chase_clock.restart();
	}
}

void pac::Enemy::updatePreviousDirections(const pac::Direction & direction)
{
	for (int i = 0; i < NUMBER_OF_SAFETY_FRAMES - 1; i++)
	{
		previous_directions[i] = previous_directions[i + 1];
	}
	previous_directions[NUMBER_OF_SAFETY_FRAMES - 1] = direction;
}

void pac::Enemy::run(const pac::Map & map)
{
	target_position = pac::Position(rand() % map.getMapSize().col * TILE_SIZE.width, rand() % map.getMapSize().row * TILE_SIZE.height);
	if (frightened_clock.getElapsedTime() >= pac::FRIGHTENED_TIME)
	{
		mode = pac::CHASE;
		move_speed = pac::ENEMY_MOVE_SPEED;
		chase_clock.restart();
	}
}

void pac::Enemy::house(pac::Map & map)
{
	if (isColliding(pac::UP, map))
	{
		direction = pac::DOWN;
	}
	else if (isColliding(pac::DOWN, map))
	{
		direction = pac::UP;
	}
}

void pac::Enemy::leaveHouse(const pac::Map & map)
{
	if (round(map.getGhostHouseCenterPosition().x - getPosition().x) == 0)
	{
		setPosition(pac::Position(map.getGhostHouseCenterPosition().x, getPosition().y));
		if (round(getPosition().y) != map.getOutsideGhostHousePosition().y)
		{
			direction = pac::UP;
			move_without_checking = true;
		}
		else
		{
			setPosition(map.getOutsideGhostHousePosition());
			mode = pac::SCATTER;
			scatter_clock.restart();
		}
	}
	else if (map.getGhostHouseCenterPosition().x - getPosition().x > 0)
	{
		direction = pac::RIGHT;
	}
	else if (map.getGhostHouseCenterPosition().x - getPosition().x < 0)
	{
		direction = pac::LEFT;
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

void pac::Enemy::frighten()
{
	mode = pac::FRIGHTENED;
	move_speed = pac::ENEMY_FRIGHTENED_MOVE_SPEED;
	direction = opposite(direction);
	frightened_clock.restart();
}