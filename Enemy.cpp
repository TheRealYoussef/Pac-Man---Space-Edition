#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Enemy.h"
#include "Map.h"
#include "Player.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Enemy::Enemy()
{
	character_type = pac::ENEMY;
	forced_switch = false;
	previous_directions.resize(pac::NUMBER_OF_SAFETY_FRAMES);
	for (int i = 0; i < pac::NUMBER_OF_SAFETY_FRAMES; i++)
	{
		previous_directions[i] = direction;
	}
	move_speed = pac::ENEMY_MOVE_SPEED;
	previous_time = sf::seconds(0.f);
	mode_clock_time.clock.restart();
}

void pac::Enemy::setScatterPosition(const pac::Map & map, const pac::Coordinate & coordinate)
{
	scatter_position = pac::Position(map.getMapPosition().x + coordinate.x * pac::TILE_SIZE.width + pac::TILE_SIZE.width / 2.f, map.getMapPosition().y + coordinate.y * pac::TILE_SIZE.height + pac::TILE_SIZE.height / 2.f);
}

void pac::Enemy::loadImages(const std::string & frightened_file_path_1, const std::string & frightened_file_path_2, const std::string & eaten_file_path)
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		frightened_images_state_1[i].texture.loadFromFile(frightened_file_path_1, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
		frightened_images_state_2[i].texture.loadFromFile(frightened_file_path_2, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
		eaten_images[i].texture.loadFromFile(eaten_file_path, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
	}
	for (int i = 1; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		frightened_images_state_1[i].texture.loadFromFile(frightened_file_path_1, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
		frightened_images_state_2[i].texture.loadFromFile(frightened_file_path_2, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
		eaten_images[i].texture.loadFromFile(eaten_file_path, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
	}
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		frightened_images_state_1[i].sprite.setTexture(frightened_images_state_1[i].texture);
		frightened_images_state_1[i].sprite.setPosition(sf::Vector2f(getPosition().x, getPosition().y));
		frightened_images_state_2[i].sprite.setTexture(frightened_images_state_2[i].texture);
		frightened_images_state_2[i].sprite.setPosition(sf::Vector2f(getPosition().x, getPosition().y));
		eaten_images[i].sprite.setTexture(frightened_images_state_1[i].texture);
		eaten_images[i].sprite.setPosition(sf::Vector2f(getPosition().x, getPosition().y));
	}
}

void pac::Enemy::targetPosition(const pac::Player & player)
{
}

void pac::Enemy::chooseBestDirection(pac::Map & map)
{
	if (mode != pac::HOUSE && mode != pac::LEAVE_HOUSE)
	{
		if (mode != pac::FRIGHTENED || (mode == pac::FRIGHTENED && previous_mode != pac::HOUSE && previous_mode != pac::LEAVE_HOUSE))
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
		if (mode != pac::FRIGHTENED)
		{
			mode = pac::LEAVE_HOUSE;
			move_speed = pac::ENEMY_HOUSE_MOVE_SPEED;
		}
		else
		{
			previous_mode = pac::LEAVE_HOUSE;
			mode = pac::FRIGHTENED;
		}
		get_out_of_ghost_house_points = -1;
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
	switchMode(pac::SCATTER, pac::SCATTER_TIME, true, true);
}

void pac::Enemy::scatter()
{
	if (forced_switch)
	{
		updatePreviousDirections(direction);
		direction = opposite(direction);
		forced_switch = false;
	}
	switchMode(pac::CHASE, pac::CHASE_TIME, false, false);
}

void pac::Enemy::switchMode(const pac::GhostMode & mode, const sf::Time & time, const bool & switch_direction, const bool & target_position)
{
	if (mode_clock_time.clock.getElapsedTime() >= mode_clock_time.time)
	{
		this->mode = mode;
		mode_clock_time.time = time;
		forced_switch = switch_direction;
		if (target_position)
		{
			this->target_position = scatter_position;
		}
		previous_time = sf::seconds(0.f);
		mode_clock_time.clock.restart();
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

void pac::Enemy::run(pac::Map & map)
{
	if (previous_mode == pac::CHASE || previous_mode == pac::SCATTER)
	{
		target_position = pac::Position(rand() % map.getMapSize().col * TILE_SIZE.width, rand() % map.getMapSize().row * TILE_SIZE.height);
	}
	else if (previous_mode == pac::HOUSE)
	{
		house(map);
	}
	else if (previous_mode == pac::LEAVE_HOUSE)
	{
		leaveHouse(map);
	}
	if (mode_clock_time.clock.getElapsedTime() >= pac::FRIGHTENED_TIME)
	{
		mode = previous_mode;
		move_speed = pac::ENEMY_MOVE_SPEED;
		mode_clock_time.clock.restart();
	}
}

void pac::Enemy::house(pac::Map & map)
{
	move_speed = pac::ENEMY_HOUSE_MOVE_SPEED;
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
			if (mode != pac::FRIGHTENED)
			{
				move_speed = pac::ENEMY_MOVE_SPEED;
				mode = pac::SCATTER;
				mode_clock_time.clock.restart();
			}
			else
			{
				move_speed = pac::ENEMY_FRIGHTENED_MOVE_SPEED;
				target_position = pac::Position(rand() % map.getMapSize().col * TILE_SIZE.width, rand() % map.getMapSize().row * TILE_SIZE.height);
				previous_mode = pac::SCATTER;
			}
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
	previous_time = mode_clock_time.clock.getElapsedTime();
	mode_clock_time.time -= previous_time;
	frightened_animation_state = pac::STATE_1;
	if (mode != pac::FRIGHTENED)
	{
		previous_mode = mode;
	}
	mode = pac::FRIGHTENED;
	if (previous_mode != pac::HOUSE && previous_mode != pac::LEAVE_HOUSE)
	{
		direction = opposite(direction);
		move_speed = pac::ENEMY_FRIGHTENED_MOVE_SPEED;
	}
	mode_clock_time.clock.restart();
}

void pac::Enemy::moveFrightened()
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		frightened_images_state_1[i].sprite.move(velocity.x, velocity.y);
		frightened_images_state_2[i].sprite.move(velocity.x, velocity.y);
		eaten_images[i].sprite.move(velocity.x, velocity.y);
	}
}

void pac::Enemy::setFrightenedPosition(const pac::Position & position)
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		frightened_images_state_1[i].sprite.setPosition(position.x, position.y);
		frightened_images_state_2[i].sprite.setPosition(position.x, position.y);
		eaten_images[i].sprite.setPosition(position.x, position.y);
	}
}

void pac::Enemy::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		if (mode == pac::FRIGHTENED)
		{
			if (frightened_animation_state == pac::STATE_1)
			{
				window.draw(frightened_images_state_1[(int)direction * 2 + (int)animation_state].sprite);
			}
			else if (frightened_animation_state == pac::STATE_2)
			{
				window.draw(frightened_images_state_2[(int)direction * 2 + (int)animation_state].sprite);
			}
		}
		else if (mode == pac::RETURN_HOUSE || mode == pac::ENTER_HOUSE)
		{
			window.draw(eaten_images[(int)direction * 2 + (int)animation_state].sprite);
		}
		else
		{
			window.draw(images[(int)direction * 2 + (int)animation_state].sprite);
		}
	}
}

void pac::Enemy::frightenedAnimation()
{
	if (mode == pac::FRIGHTENED)
	{
		if (mode_clock_time.clock.getElapsedTime() >= pac::TIME_FOR_ENEMY_TO_START_BLINKING)
		{
			frightened_animation_state = (frightened_animation_state == pac::STATE_1) ? pac::STATE_2 : pac::STATE_1;
		}
	}
}