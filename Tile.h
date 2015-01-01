#include "SFML/Graphics.hpp"

#include "GlobalVariables.h"

#ifndef Tile_h
#define Tile_h

class Player;

class Tile
{

private:
	
	//The position of the tile relative to the SFML window
	Position position;
	
	//The tile boxes of the tile to indicate which parts of the tile contain a wall
	TileBox tile_boxes[2][2];
	
	//The tile's sf::Texture and sf::Sprite
	TextureSprite image;

public:

	//The type of the tile
	TileType tile_type;

	//Default constructor, used only for operator=
	Tile();
	
	//Construct a tile
	//int tile_number: the number of the tile to load the correct sf::Texture
	//Position tile_position: the position of the tile relative to the SFML window
	//TileType tile_type: the type of the tile. Can be { WALL, NORMAL, INTERSECTION, TELEPORTATION, DOOR }
	//const TileBox tile_boxes[2][2]: a tile is split into 4 boxes. Each box can have a wall or not and has a Position
	//sf::Texture & tile_texture: the texture of the tile
	Tile(const Position & tile_position, const TileType & tile_type, const TileBox tile_boxes[2][2], sf::Texture & tile_texture);
	
	//Copy a tile
	Tile& operator=(const Tile & tile);
	
	//Return true if the object is colliding with the tile to the object's right, false otherwise
	bool isCollidingRight(Player & object) const;
	
	//Return true if the object is colliding with the tile to the object's left, false otherwise
	bool isCollidingLeft(Player & object) const;
	
	//Return true if the object is colliding with the tile above the object, false otherwise
	bool isCollidingUp(Player & object) const;
	
	//Return true if the object is colliding with the tile below the object, false otherwise
	bool isCollidingDown(Player & object) const;
	
	//Display the tile
	void display(sf::RenderWindow & window) const;

};

#endif
