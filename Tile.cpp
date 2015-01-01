#include "SFML/Graphics.hpp"

#include <string>
#include <fstream>
#include <cmath>
#include <iostream>

#include "Player.h"
#include "Tile.h"
#include "GlobalVariables.h"

Tile::Tile()
{
}

Tile::Tile(const Position & tile_position, const TileType & tile_type, const TileBox tile_boxes[2][2], sf::Texture & tile_texture)
{
	position.x = tile_position.x;
	position.y = tile_position.y;
	this->tile_type = tile_type;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			this->tile_boxes[i][j].position.x = tile_boxes[i][j].position.x;
			this->tile_boxes[i][j].position.y = tile_boxes[i][j].position.y;
			this->tile_boxes[i][j].wall = tile_boxes[i][j].wall;
		}
	}
	image.texture = &tile_texture;
}

Tile& Tile::operator=(const Tile & T)
{
	position.x = T.position.x;
	position.y = T.position.y;
	tile_type = T.tile_type;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			tile_boxes[i][j].position.x = T.tile_boxes[i][j].position.x;
			tile_boxes[i][j].position.y = T.tile_boxes[i][j].position.y;
			tile_boxes[i][j].wall = T.tile_boxes[i][j].wall;
		}
	}
	image.sprite.setTexture(*(T.image.texture));
	image.sprite.setScale(TILE_SCALE.x, TILE_SCALE.y);
	image.sprite.setPosition(T.position.x, T.position.y);
	return *this;
}

void Tile::display(sf::RenderWindow & window) const
{
	window.draw(image.sprite);
}

bool Tile::isCollidingRight(Player & object) const
{
	float extra_collision_distance = object.getTimePerFrame() * 1000.0;
	for (int i = 0; i < TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < TILE_BOXES_COLS; j++)
		{
			if (tile_boxes[i][j].wall)
			{
				if (round(object.getPosition().x) + CHARACTER_SIZE.width + extra_collision_distance >= tile_boxes[i][j].position.x && object.getPosition().x < tile_boxes[i][j].position.x + TILE_BOX.width && round(object.getPosition().y) < tile_boxes[i][j].position.y + TILE_BOX.height && round(object.getPosition().y) + CHARACTER_SIZE.height > tile_boxes[i][j].position.y)
				{
					if (object.getPosition().x + CHARACTER_SIZE.width != tile_boxes[i][j].position.x)
						object.setPosition(Position{ tile_boxes[i][j].position.x - CHARACTER_SIZE.width, object.getPosition().y });
					return true;
				}
			}
		}
	}
	return false;
}

bool Tile::isCollidingLeft(Player & object) const
{
	float extra_collision_distance = object.getTimePerFrame() * 1000.0;
	for (int i = 0; i < TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < TILE_BOXES_COLS; j++)
		{
			if (tile_boxes[i][j].wall)
			{
				if (round(object.getPosition().x) - extra_collision_distance <= tile_boxes[i][j].position.x + TILE_BOX.width && object.getPosition().x + CHARACTER_SIZE.width > tile_boxes[i][j].position.x && round(object.getPosition().y) < tile_boxes[i][j].position.y + TILE_BOX.height && round(object.getPosition().y) + CHARACTER_SIZE.height > tile_boxes[i][j].position.y)
				{
					if (object.getPosition().x != tile_boxes[i][j].position.x + TILE_BOX.width)
						object.setPosition(Position{ tile_boxes[i][j].position.x + TILE_BOX.width, object.getPosition().y });
					return true;
				}
			}
		}
	}
	return false;
}

bool Tile::isCollidingUp(Player & object) const
{ 
	float extra_collision_distance = object.getTimePerFrame() * 1000.0;
	for (int i = 0; i < TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < TILE_BOXES_COLS; j++)
		{
			if (tile_boxes[i][j].wall)
			{
				if (round(object.getPosition().y) - extra_collision_distance <= tile_boxes[i][j].position.y + TILE_BOX.height && object.getPosition().y + CHARACTER_SIZE.height > tile_boxes[i][j].position.y && round(object.getPosition().x) < tile_boxes[i][j].position.x + TILE_BOX.width && round(object.getPosition().x) + CHARACTER_SIZE.width > tile_boxes[i][j].position.x)
				{
					if (object.getPosition().y != tile_boxes[i][j].position.y + TILE_BOX.width)
						object.setPosition(Position{ object.getPosition().x, tile_boxes[i][j].position.y + TILE_BOX.height });
					return true;
				}
			}
		}
	}
	return false;
}

bool Tile::isCollidingDown(Player & object) const
{
	float extra_collision_distance = object.getTimePerFrame() * 1000.0;
	for (int i = 0; i < TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < TILE_BOXES_COLS; j++)
		{
			if (tile_boxes[i][j].wall)
			{
				if (round(object.getPosition().y) + CHARACTER_SIZE.height + extra_collision_distance >= tile_boxes[i][j].position.y && object.getPosition().y < tile_boxes[i][j].position.y + TILE_BOX.height && round(object.getPosition().x) < tile_boxes[i][j].position.x + TILE_BOX.width && round(object.getPosition().x) + CHARACTER_SIZE.width > tile_boxes[i][j].position.x)
				{
					if (object.getPosition().y + CHARACTER_SIZE.height != tile_boxes[i][j].position.y)
						object.setPosition(Position{ object.getPosition().x, tile_boxes[i][j].position.y - CHARACTER_SIZE.height });
					return true;
				}
			}
		}
	}
	return false;
}
