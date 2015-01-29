#include "SFML/Graphics.hpp"

#ifndef GlobalVariables_h
#define GlobalVariables_h

namespace pac
{
	//Position of the object relative to the SFML window
	class Position
	{
	public:
		Position(); 
		Position(const float & x, const float & y);
		float x;
		float y;
		Position& operator=(const Position & position);
		Position& operator=(const sf::Vector2f & position);
		const Position operator+(const Position & position) const;
		bool operator!=(const Position & position) const;
	};

	//Object's width and height in pixels
	class Size
	{
	public:
		Size();
		Size(const float & width, const float & height);
		float width;
		float height;
		Size& operator=(const Size & size);
	};

	//Object's size scaling in the x and y directions
	class Scale
	{
	public:
		Scale();
		Scale(const float & x, const float & y);
		float x;
		float y;
		Scale& operator=(const Scale & scale);
	};

	//Distance in the x and y directions
	class Distance
	{
	public:
		Distance();
		Distance(const float & x, const float & y);
		float x;
		float y;
		Distance& operator=(const Distance & distance);
	};

	//Object's sf::texture and sf::sprite
	class Image
	{
	public:
		Image();
		sf::Texture texture;
		sf::Sprite sprite;
	};

	//The way the button is changed to indicate it is selected
	//IMAGE: an image of the selected button is displayed instead of the normal button
	//SCALE: the size of the image of the button is scaled
	//IMAGE_SCALE: a combination of the two other transition types. An image of the selected is displayed instead of the normal button and its size is scaled
	enum TransitionType { IMAGE, SCALE, IMAGE_SCALE };

	//Tile is scaled by 2 in the x and y directions
	const Scale TILE_SCALE( 2.0f, 2.0f );

	//Tile size is 8 pixels x 8 pixels
	//Tile length and width is scaled by 2
	//Tile size changes to 16 pixels x 16 pixels 
	const Size TILE_SIZE( 16.0f, 16.0f );

	//Defines void functions with 0 input arguments
	typedef void(*FUNCTIONS) ();

	//The type of the tile
	//Can be { WALL, NORMAL, INTERSECTION, TELEPORTATION, DOOR }
	//WALL: tile contains a wall
	//NORMAL: normal tile for walking
	//INTERSECTION: normal tile for walking but is an intersection between 3 or more paths
	//TELEPORTATION: special tile which teleports the object standing on it to another TELEPORTATION tile linked to it
	//DOOR: special tile which can be passed through in one direction only
	enum TileType { WALL, NORMAL, TELEPORTATION, DOOR, NO_TYPE };

	//A part of a tile
	//A tile has 4 TileBox
	//A tile box has a Position and can have a wall or not
	class TileBox
	{
	public:
		TileBox();
		Position position;
		bool wall;
		TileBox& operator=(const TileBox & tile_box);
	};

	//Size of a grid
	//int row: number of cells in the row
	//int col: number of cells in the column
	class GridSize
	{
	public:
		GridSize();
		GridSize(const int & row, const int & col);
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
	const Size TILE_BOX( 8.f, 8.f );

	//A character's size (player or enemy) is 32 pixels x 32 pixels
	const Size CHARACTER_SIZE( 32.f, 32.f );

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
	class ClockTime
	{
	public:
		ClockTime();
		sf::Clock clock;
		sf::Time time;
	};

	//Velocity of an object
	//float x: velocity in the x direction
	//float y: velocity in the y direction
	class Velocity
	{
	public:
		Velocity();
		Velocity(const float & x, const float & y);
		float x;
		float y;
		Velocity& operator=(const Velocity & velocity);
		Velocity& operator=(const int & value);
		bool operator!=(const int & value);
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
	class Coordinate
	{
	public:
		Coordinate();
		Coordinate(const int & x, const int & y);
		int x;
		int y;
		Coordinate& operator=(const Coordinate & coordinate);
	};

	//A character's width can cover a maximum of 3 tiles, same goes for height
	const int MAXIMUM_CHARACTER_TILE_COVERAGE = 3;

	//A point can be either eaten or not
	enum EatenState { EATEN, NOT_EATEN };

	//The information needed to create a TeleportationPair (comes from map file)
	class TeleportationPairInfo
	{
	public:
		TeleportationPairInfo();
		TeleportationPairInfo(const Coordinate & teleporter_1_c, const Direction & teleporter_1_d, const Coordinate & teleporter_2_c, const Direction & teleporter_2_d);
		Coordinate teleporter_1_c;
		Direction teleporter_1_d;
		Coordinate teleporter_2_c;
		Direction teleporter_2_d;
	};

	//The time between each switch of the player's animation states
	const sf::Time PLAYER_ANIMATION_TIME = sf::seconds(0.1f);

	//The time between each switch of the ghost's animation states
	const sf::Time GHOST_ANIMATION_TIME = sf::seconds(0.2f);

	//Right, Left, Up, Down
	const int NUMBER_OF_POSSIBLE_DIRECTIONS = 4;

	//The number of tiles infront of the player that Pinky targets
	const int PINKY_TARGETING_DISTANCE = 4;

	//If Clyde is closer than 8 tiles from the player, then he moves away. Otherwise, he moves to the player's position
	const int CLYDE_MAXIMUM_DISTANCE_FROM_PLAYER = 8;

	//A tile is a square with side == 16 pixels
	const float TILE_SIDE_SIZE = 16.f;

	//A ghost has 3 modes
	//CHASE: follows the player using its unique targeting system
	//SCATTER: targets its own corner of the map
	//FRIGHTENED: moves in a random direction
	//HOUSE: moves up and down until given a command to leave the ghost house
	enum GhostMode {CHASE, SCATTER, FRIGHTENED, HOUSE, LEAVE_HOUSE};

	//The amount of time the ghosts stay in scatter mode
	const sf::Time SCATTER_TIME = sf::seconds(3.f);

	//The amount of time the ghosts stay in chase mode
	const sf::Time CHASE_TIME = sf::seconds(15.f);

	//The amount of time the ghosts stay in frightened mode
	const sf::Time FRIGHTENED_TIME = sf::seconds(7.f);

	//Maximum frames per second allowed
	const int MAX_FRAMES_PER_SECOND = 240;

	//Lowest time per frame allowed
	const float LOWEST_TIME_PER_FRAME = 1.f / MAX_FRAMES_PER_SECOND;

	//Player's movement speed (pixels per second)
	const float PLAYER_MOVE_SPEED = 100.f;

	//Enemy's normal movement speed
	const float ENEMY_MOVE_SPEED = 100.f;

	//Enemy's movement speed in frightened mode
	const float ENEMY_FRIGHTENED_MOVE_SPEED = 60.f;

	//Number of safety frames which depends on the enemies' lowest movement speed to prevent the enemies from glitching
	const int NUMBER_OF_SAFETY_FRAMES = ((int)(0.96f / (ENEMY_FRIGHTENED_MOVE_SPEED * LOWEST_TIME_PER_FRAME)) < 0.96f / (ENEMY_FRIGHTENED_MOVE_SPEED * LOWEST_TIME_PER_FRAME)) ? 0.96f / (ENEMY_FRIGHTENED_MOVE_SPEED * LOWEST_TIME_PER_FRAME) + 1 : 0.96f / (ENEMY_FRIGHTENED_MOVE_SPEED * LOWEST_TIME_PER_FRAME);

	//Type of character
	//PLAYER: player (value 0)
	//ENEMY: enemy (value 1)
	enum CharacterType{ PLAYER, ENEMY };

	//The number of points the player has to eat for Pinky to get out of the ghost house
	const int PINKY_GET_OUT_OF_GHOST_HOUSE_POINTS = 1;

	//The number of points the player has to eat for Inky to get out of the ghost house
	const int INKY_GET_OUT_OF_GHOST_HOUSE_POINTS = 20;

	//The number of points the player has to eat for Clyde to get out of the ghost house
	const int CLYDE_GET_OUT_OF_GHOST_HOUSE_POINTS = 50;

}

#endif