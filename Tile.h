#include "SFML/Graphics.hpp"

#include "GlobalVariables.h"

#ifndef Tile_h
#define Tile_h

namespace pac
{
	class Tile
	{

	private:

		//The position of the tile relative to the SFML window
		Position position;

		//The type of the tile
		TileType tile_type;

		//The tile boxes of the tile to indicate which parts of the tile contain a wall
		TileBox tile_boxes[TILE_BOXES_ROWS][TILE_BOXES_COLS];

		//The tile's sf::Sprite
		sf::Sprite image;

	public:

		//Default constructor
		Tile();

		//Initialize the tile
		//int tile_number: the number of the tile to load the correct sf::Texture
		//Position tile_position: the position of the tile relative to the SFML window
		//TileType tile_type: the type of the tile. Can be { WALL, NORMAL, INTERSECTION, TELEPORTATION, DOOR }
		//const TileBox tile_boxes[2][2]: a tile is split into 4 boxes. Each box can have a wall or not and has a Position
		//sf::Texture & tile_texture: the texture of the tile
		void init(const Position & tile_position, const TileType & tile_type, const TileBox tile_boxes[TILE_BOXES_ROWS][TILE_BOXES_COLS], sf::Texture & tile_texture);

		//Display the tile
		void display(sf::RenderWindow & window) const;

		//Get the tile type
		TileType getTileType() const;

		//Return pointers to the tile boxes of the tile
		void getTileBoxes(TileBox *tile_boxes[TILE_BOXES_ROWS][TILE_BOXES_COLS]);

		//Destructor
		~Tile();

	};
}

#endif