#include "SFML/Graphics.hpp"

#include <cmath>
#include <iostream>

#include "Character.h"
#include "Map.h"
#include "Tile.h"
#include "TeleportationPair.h"

#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Character::Character()
{
	move_without_checking = false;
	animation_state = pac::STATE_1;
}

void pac::Character::init(const pac::Position & spawn_position, const std::string & file_path, pac::Map & map)
{
	spawn = spawn_position;
	map.getTeleportationPairs(teleportation_pairs);
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		images[i].texture.loadFromFile(file_path, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
	}
	for (int i = 1; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		images[i].texture.loadFromFile(file_path, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
	}
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		images[i].sprite.setTexture(images[i].texture);
		images[i].sprite.setPosition(spawn_position.x, spawn_position.y);
	}
}

void pac::Character::move(pac::Map & map, const float & time_per_frame)
{
	setCharacterVelocity(map, time_per_frame);
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		images[i].sprite.move(velocity.x, velocity.y);
	}
	if (character_type == pac::ENEMY)
	{
		moveFrightened();
	}
}

void pac::Character::moveFrightened()
{
}

void pac::Character::teleport()
{
	for (unsigned int i = 0; i < teleportation_pairs.size(); i++)
	{
		teleportation_pairs[i]->teleport(*this);
	}
}

void pac::Character::setCharacterVelocity(pac::Map & map, const float & time_per_frame)
{
	switch (direction)
	{
	case pac::RIGHT:
		setVelocity(pac::Velocity(move_speed * time_per_frame, 0), map);
		break;
	case pac::LEFT:
		setVelocity(pac::Velocity(-move_speed * time_per_frame, 0), map);
		break;
	case pac::UP:
		setVelocity(pac::Velocity(0, -move_speed * time_per_frame), map);
		break;
	case pac::DOWN:
		setVelocity(pac::Velocity(0, move_speed * time_per_frame), map);
		break;
	}
}

void pac::Character::setVelocity(const pac::Velocity & velocity, Map & map)
{
	if (move_without_checking)
	{
		move_without_checking = false;
		this->velocity = velocity;
	}
	else if (!isColliding(direction, map))
	{
		this->velocity = velocity;
	}
	else
	{
		this->velocity = 0;
	}
}

bool pac::Character::isColliding(const Direction & direction, pac::Map & map)
{
	coordinate = map.getCoordinate(getPosition());
	if (map.getCollidingTiles(coordinate, direction, tiles))
	{
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			tiles[i]->getTileBoxes(tile_boxes[i]);
		}
		for (int i = 0; i < pac::DISTINCT_COLLIDING_TILES; i++)
		{
			if (checkTileBoxes(tile_boxes[i], direction))
			{
				return true;
			}
		}
	}
	return false;
}

bool pac::Character::checkTileBoxes(pac::TileBox *tile_boxes[TILE_BOXES_ROWS][TILE_BOXES_COLS], const pac::Direction & direction)
{
	for (int i = 0; i < pac::TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < pac::TILE_BOXES_COLS; j++)
		{
			if (tile_boxes[i][j]->wall)
			{
				if (checkForCollision(direction, tile_boxes[i][j]->position))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool pac::Character::checkForCollision(const pac::Direction & direction, const pac::Position & tile_box_position)
{
	setCharacterSides();
	setTileBoxSides(tile_box_position);
	switch (direction)
	{
	case RIGHT:
		return checkForCollisionRight();
	case LEFT:
		return checkForCollisionLeft();
	case UP:
		return checkForCollisionUp();
	case DOWN:
		return checkForCollisionDown();
	}
}

bool pac::Character::checkForCollisionRight()
{
	if (character_right >= tile_box_left && character_left < tile_box_right && character_up < tile_box_down && character_down > tile_box_up)
	{
		if (character_right != tile_box_left)
		{
			setPosition(pac::Position(tile_box_left - CHARACTER_SIZE.width, getPosition().y));
		}
		return true;
	}
	return false;
}

bool pac::Character::checkForCollisionLeft()
{
	if (character_left <= tile_box_right && character_right > tile_box_left && character_up < tile_box_down && character_down > tile_box_up)
	{
		if (character_left != tile_box_right)
		{
			setPosition(pac::Position(tile_box_right, getPosition().y));
		}
		return true;
	}
	return false;
}

bool pac::Character::checkForCollisionUp()
{
	if (character_up <= tile_box_down && character_down > tile_box_up && character_left < tile_box_right && character_right > tile_box_left)
	{
		if (character_up != tile_box_down)
		{
			setPosition(pac::Position(getPosition().x, tile_box_down));
		}
		return true;
	}
	return false;
}

bool pac::Character::checkForCollisionDown()
{
	if (character_down >= tile_box_up && character_up < tile_box_down && character_left < tile_box_right && character_right > tile_box_left)
	{
		if (character_down != tile_box_up)
		{
			setPosition(pac::Position(getPosition().x, tile_box_up - CHARACTER_SIZE.height));
		}
		return true;
	}
	return false;
}

void pac::Character::setCharacterSides()
{
	character_right = round(getPosition().x) + pac::CHARACTER_SIZE.width;
	character_left = round(getPosition().x);
	character_up = round(getPosition().y);
	character_down = round(getPosition().y) + pac::CHARACTER_SIZE.height;
}

void pac::Character::setTileBoxSides(const pac::Position & tile_box_position)
{
	tile_box_right = tile_box_position.x + pac::TILE_BOX.width;
	tile_box_left = tile_box_position.x;
	tile_box_up = tile_box_position.y;
	tile_box_down = tile_box_position.y + pac::TILE_BOX.height;
}

void pac::Character::playAnimation()
{
	animation_state = (animation_state == pac::STATE_1) ? pac::STATE_2 : pac::STATE_1;
}

void pac::Character::setDirection(const pac::Direction & direction)
{
	this->direction = direction;
}


pac::Direction pac::Character::getDirection() const
{
	return direction;
}


void pac::Character::setPosition(const pac::Position & position)
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		images[i].sprite.setPosition(position.x, position.y);
	}
	if (character_type == pac::ENEMY)
	{
		setFrightenedPosition(position);
	}
}

void pac::Character::setFrightenedPosition(const pac::Position & position)
{
}

pac::Position pac::Character::getPosition() const
{
	return pac::Position(images[0].sprite.getPosition().x, images[0].sprite.getPosition().y);
}

pac::CharacterType pac::Character::getCharacterType() const
{
	return character_type;
}

pac::Character::~Character()
{
}