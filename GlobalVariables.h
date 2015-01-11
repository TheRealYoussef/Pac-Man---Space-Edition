#include "SFML/Graphics.hpp"

#ifndef GlobalVariables_h
#define GlobalVariables_h

namespace pac
{
	//Position of the object relative to the SFML window
	struct Position
	{
		float x;
		float y;
		Position& operator=(const Position & position) { x = position.x; y = position.y; return *this; }
		Position& operator=(const sf::Vector2f & position) { x = position.x; y = position.y; return *this; }
		Position operator+(const Position & position) { float x1 = x + position.x; float y1 = y + position.y; return Position{ x1, y1 }; }
	};

	//Object's width and height in pixels
	struct Size
	{
		float width;
		float height;
		Size& operator=(const Size & size) { width = size.width; height = size.height; return *this; }
	};

	//Object's size scaling in the x and y directions
	struct Scale
	{
		float x;
		float y;
		Scale& operator=(const Scale & scale) { x = scale.x; y = scale.y; return *this; }
	};

	//Distance in the x and y directions
	struct Distance
	{
		float x;
		float y;
		Distance& operator=(const Distance & distance) { x = distance.x; y = distance.y; return *this; }
	};

	//Object's sf::texture and sf::sprite
	struct Image
	{
		sf::Texture texture;
		sf::Sprite sprite;
	};

	//The way the button is changed to indicate it is selected
	//IMAGE: an image of the selected button is displayed instead of the normal button
	//SCALE: the size of the image of the button is scaled
	//IMAGE_SCALE: a combination of the two other transition types. An image of the selected is displayed instead of the normal button and its size is scaled
	enum TransitionType { IMAGE, SCALE, IMAGE_SCALE };

	//Tile is scaled by 2 in the x and y directions
	const Scale TILE_SCALE{ 2.0f, 2.0f };

	//Tile size is 8 pixels x 8 pixels
	//Tile length and width is scaled by 2
	//Tile size changes to 16 pixels x 16 pixels 
	const Size TILE_SIZE{ 16.0f, 16.0f };

	//Defines void functions with 0 input arguments
	typedef void(*FUNCTIONS) ();

	//The type of the tile
	//Can be { WALL, NORMAL, INTERSECTION, TELEPORTATION, DOOR }
	//WALL: tile contains a wall
	//NORMAL: normal tile for walking
	//INTERSECTION: normal tile for walking but is an intersection between 3 or more paths
	//TELEPORTATION: special tile which teleports the object standing on it to another TELEPORTATION tile linked to it
	//DOOR: special tile which can be passed through in one direction only
	enum TileType { WALL, NORMAL, INTERSECTION, TELEPORTATION, DOOR, NO_TYPE };

	//A part of a tile
	//A tile has 4 TileBox
	//A tile box has a Position and can have a wall or not
	struct TileBox
	{
		Position position;
		bool wall;
		TileBox& operator=(const TileBox & tile_box) { position = tile_box.position; wall = tile_box.wall; return *this; }
	};

	//Size of a grid
	//int row: number of cells in the row
	//int col: number of cells in the column
	struct GridSize
	{
		int row;
		int col;
	};

	//A tile has 2 rows of tile boxes
	const int TILE_BOXES_ROWS = 2;

	//A tile has 2 cols of tile boxes
	const int TILE_BOXES_COLS = 2;

	//Subtract a character (digit) ascii by 48 to change it to an integer
	const int ASCII_CHAR_TO_INT_DIFF = 48;

	//A tile box's size is 8 pixels x 8 pixels
	const Size TILE_BOX{ 8.0f, 8.0f };

	//A character's size (player or enemy) is 32 pixels x 32 pixels
	const Size CHARACTER_SIZE{ 32.0f, 32.0f };

	//Direction
	//RIGHT: right (value 0)
	//LEFT: left (value 1)
	//UP: up (value 2)
	//DOWN: down (value 3)
	//NONE: no direction (value 4)
	enum Direction{ RIGHT, LEFT, UP, DOWN, NONE };

	//Animations can have 2 states
	//state_1
	//state_2
	enum AnimationState{ STATE_1, STATE_2 };

	//sf::Clock and sf::Time pair
	struct ClockTime
	{
		sf::Clock clock;
		sf::Time time;
	};

	//Velocity of an object
	//float x: velocity in the x direction
	//float y: velocity in the y direction
	struct Velocity
	{
		float x;
		float y;
		Velocity& operator=(const Velocity & velocity) { x = velocity.x; y = velocity.y; return *this; }
		Velocity& operator=(const int & value) { x = value; y = value; return *this; }
		bool operator!=(const int & value) { return (x != value || y != value); }
	};

	//The types of points that the player can eat
	//NORMAL_POINT: worth 10
	//POWER_POINT: worth 50, triggers eating enemies event for a short period of time
	//RARE_POINT: worth varies
	//NO_POINT: no point
	enum PointType { NORMAL_POINT, POWER_POINT, RARE_POINT, NO_POINT };

	//The distance between the player's sprite borders and the actual image of the player 
	const float ACTUAL_EATING_DISTANCE = 1.0f;

	//The distance between the normal point's sprite borders and the actual image of the normal point
	const float NORMAL_POINT_EATING_DISTANCE = 8.0f;

	//A character has a total of 8 sprites, 2 for each direction he is facing
	const int NUMBER_OF_SPRITES_PER_CHARACTER = 8;

	//A character can collide with only 3 tiles in a single direction
	const int DISTINCT_COLLIDING_TILES = 3;

	//Pair
	const int PAIR = 2;

	//Coordinate in the map
	struct Coordinate
	{
		int x;
		int y;
		Coordinate& operator=(const Coordinate & coordinate) { x = coordinate.x; y = coordinate.y; return *this; }
	};

	//A character's width can cover a maximum of 3 tiles, same goes for height
	const int MAXIMUM_CHARACTER_TILE_COVERAGE = 3;

	//A point can be either eaten or not
	enum EatenState { EATEN, NOT_EATEN };

	//The information needed to create a TeleportationPair (comes from map file)
	struct TeleportationPairInfo
	{
		Coordinate teleporter_1_c;
		Direction teleporter_1_d;
		Coordinate teleporter_2_c;
		Direction teleporter_2_d;
	};

	//
	const sf::Time player_animation_time = sf::seconds(0.15f);

}

#endif