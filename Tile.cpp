#include "SFML/Graphics.hpp"

#include <string>
#include <fstream>
#include <cmath>
#include <iostream>

#include "Player.h"
#include "Tile.h"
#include "GlobalVariables.h"

pac::Tile::Tile()
{
	tile_type = pac::NO_TYPE;
}

void pac::Tile::init(const pac::Position & tile_position, const pac::TileType & tile_type, const pac::TileBox tile_boxes[2][2], sf::Texture & texture)
{
	position = tile_position;
	this->tile_type = tile_type;
	for (int i = 0; i < pac::TILE_BOXES_ROWS; i++)
	{
		for (int j = 0; j < pac::TILE_BOXES_COLS; j++)
		{
			this->tile_boxes[i][j] = tile_boxes[i][j];
		}
	}
	image.setTexture(texture);
	image.setScale(TILE_SCALE.x, TILE_SCALE.y);
	image.setPosition(position.x, position.y);
}


void pac::Tile::display(sf::RenderWindow & window) const
{
	if (tile_type != pac::NO_TYPE)
	{
		window.draw(image);
	}
}

void pac::Tile::getTileBoxes(pac::TileBox *tile_boxes[pac::TILE_BOXES_ROWS][pac::TILE_BOXES_COLS])
{
	if (tile_type != pac::NO_TYPE)
	{
		for (int i = 0; i < pac::TILE_BOXES_ROWS; i++)
		{
			for (int j = 0; j < pac::TILE_BOXES_COLS; j++)
			{
				tile_boxes[i][j] = &(this->tile_boxes[i][j]);
			}
		}
	}
}

pac::TileType pac::Tile::getTileType() const
{
	return tile_type;
}

pac::Tile::~Tile()
{
}