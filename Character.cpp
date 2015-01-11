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
	moved = true;
	move_without_checking = false;
	animation_state = pac::STATE_2;
	animation.clock.restart();
	movement.clock.restart();
}

void pac::Character::move(pac::Map & map, const float & time_per_frame)
{
	setCharacterVelocity(time_per_frame, map);
	movement.time = sf::seconds(time_per_frame);
	if (movement.clock.getElapsedTime() >= movement.time)
	{
		for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
		{
			images[i].sprite.move(velocity.x, velocity.y);
		}
		moved = true;
		movement.clock.restart();
	}
	for (int i = 0; i < teleportation_pairs.size(); i++)
	{
		teleportation_pairs[i]->teleport(*this);
	}
}

void pac::Character::setCharacterVelocity(const float & time_per_frame, pac::Map & map)
{
	switch (direction)
	{
	case pac::RIGHT:
		setVelocity(pac::Velocity{ move_speed * time_per_frame, 0 }, time_per_frame, map);
		break;
	case pac::LEFT:
		setVelocity(pac::Velocity{ -move_speed * time_per_frame, 0 }, time_per_frame, map);
		break;
	case pac::UP:
		setVelocity(pac::Velocity{ 0, -move_speed * time_per_frame }, time_per_frame, map);
		break;
	case pac::DOWN:
		setVelocity(pac::Velocity{ 0, move_speed * time_per_frame }, time_per_frame, map);
		break;
	}
}

void pac::Character::setVelocity(const pac::Velocity & velocity, const float & time_per_frame, Map & map)
{
	if (move_without_checking)
	{
		move_without_checking = false;
		this->velocity = velocity;
	}
	else if (!isColliding(direction, time_per_frame, map))
	{
		this->velocity = velocity;
	}
	else
	{
		this->velocity = 0;
	}
}

bool pac::Character::isColliding(const Direction & direction, const float & time_per_frame, pac::Map & map)
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
			for (int j = 0; j < pac::TILE_BOXES_ROWS; j++)
			{
				for (int k = 0; k < pac::TILE_BOXES_COLS; k++)
				{
					if (tile_boxes[i][j][k]->wall)
					{
						switch (direction)
						{
						case RIGHT:
							if (biggerThanOrEqual(round(getPosition().x) + pac::CHARACTER_SIZE.width + time_per_frame, tile_boxes[i][j][k]->position.x) && lessThan(getPosition().x, tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width) && lessThan(round(getPosition().y), tile_boxes[i][j][k]->position.y + pac::TILE_BOX.height) && biggerThan(round(getPosition().y) + pac::CHARACTER_SIZE.height, tile_boxes[i][j][k]->position.y))
							{
								if (notEqual(getPosition().x + pac::CHARACTER_SIZE.width, tile_boxes[i][j][k]->position.x))
								{
									setPosition(pac::Position{ tile_boxes[i][j][k]->position.x - pac::CHARACTER_SIZE.width, getPosition().y });
								}
								return true;
							}
							break;
						case LEFT:
							if (lessThanOrEqual(round(getPosition().x) - time_per_frame, tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width) && biggerThan(getPosition().x + pac::CHARACTER_SIZE.width, tile_boxes[i][j][k]->position.x) && lessThan(round(getPosition().y), tile_boxes[i][j][k]->position.y + pac::TILE_BOX.height) && biggerThan(round(getPosition().y) + pac::CHARACTER_SIZE.height, tile_boxes[i][j][k]->position.y))
							{
								if (notEqual(getPosition().x, tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width))
								{
									setPosition(pac::Position{ tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width, getPosition().y });
								}
								return true;
							}
							break;
						case UP:
							if (lessThanOrEqual(round(getPosition().y) - time_per_frame, tile_boxes[i][j][k]->position.y + pac::TILE_BOX.height) && biggerThan(getPosition().y + pac::CHARACTER_SIZE.height, tile_boxes[i][j][k]->position.y) && lessThan(round(getPosition().x), tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width) && biggerThan(round(getPosition().x) + pac::CHARACTER_SIZE.width, tile_boxes[i][j][k]->position.x))
							{
								if (notEqual(getPosition().y, tile_boxes[i][j][k]->position.y + pac::TILE_BOX.width))
								{
									setPosition(pac::Position{ getPosition().x, tile_boxes[i][j][k]->position.y + pac::TILE_BOX.height });
								}
								return true;
							}
							break;
						case DOWN:
							if (biggerThanOrEqual(round(getPosition().y) + pac::CHARACTER_SIZE.height + time_per_frame, tile_boxes[i][j][k]->position.y) && lessThan(getPosition().y, tile_boxes[i][j][k]->position.y + pac::TILE_BOX.height) && lessThan(round(getPosition().x), tile_boxes[i][j][k]->position.x + pac::TILE_BOX.width) && biggerThan(round(getPosition().x) + pac::CHARACTER_SIZE.width, tile_boxes[i][j][k]->position.x))
							{
								if (notEqual(getPosition().y + pac::CHARACTER_SIZE.height, tile_boxes[i][j][k]->position.y))
								{
									setPosition(pac::Position{ getPosition().x, tile_boxes[i][j][k]->position.y - pac::CHARACTER_SIZE.height });
								}
								return true;
							}
							break;
						}
					}
				}
			}
		}
	}
	return false;
}

void pac::Character::playAnimation()
{
	if (animation.clock.getElapsedTime() >= animation.time)
	{
		animation_state = (animation_state == pac::STATE_1) ? pac::STATE_2 : pac::STATE_1;
		animation.clock.restart();
	}
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
}


pac::Position pac::Character::getPosition() const
{
	return pac::Position{ images[0].sprite.getPosition().x, images[0].sprite.getPosition().y };
}


void pac::Character::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		window.draw(images[direction * 2 + animation_state].sprite);
	}
}

pac::Character::~Character()
{
}