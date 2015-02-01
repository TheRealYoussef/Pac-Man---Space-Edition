#include "SFML/Graphics.hpp"

#include "Tile.h"
#include "TeleportationPair.h"
#include "Map.h"
#include "GlobalVariables.h"

#ifndef Character_h
#define Character_h

namespace pac
{
	class Character
	{

	private:

		//Pointers to the 3 tiles the character is colliding with in the direction he is moving
		Tile *tiles[DISTINCT_COLLIDING_TILES];

		//Pointers to the tile boxes of the 3 tiles the character is colliding with in the direction he is moving
		TileBox *tile_boxes[DISTINCT_COLLIDING_TILES][TILE_BOXES_ROWS][TILE_BOXES_COLS];

		//A vector of pointers to all the teleportation pairs in the map
		std::vector <TeleportationPair*> teleportation_pairs;

		//Character's sides
		float character_right, character_left, character_up, character_down;

		//Tile box's sides
		float tile_box_right, tile_box_left, tile_box_up, tile_box_down;

		//Set the character's velocity based on his direction and collision
		void setCharacterVelocity(Map & map, const float & time_per_frame);

		//Set the velocity
		void setVelocity(const Velocity & velocity, Map & map);

		//Check the tile boxes of a tile for collision
		bool checkTileBoxes(TileBox *tile_boxes[TILE_BOXES_ROWS][TILE_BOXES_COLS], const Direction & direction);

		//Check a single tile box for collision
		bool checkForCollision(const Direction & direction, const Position & tile_box_position);

		//Check for collision to the right
		bool checkForCollisionRight();

		//Check for collision to the left
		bool checkForCollisionLeft();

		//Check for collision up
		bool checkForCollisionUp();

		//Check for collision down
		bool checkForCollisionDown();

		//Set the character's sides
		void setCharacterSides();

		//Set the tile box's sides
		void setTileBoxSides(const Position & tile_box_position);

	protected:

		//The images of the differect sprites of the character
		Image images[NUMBER_OF_SPRITES_PER_CHARACTER];

		//The current state of the character's animation
		AnimationState animation_state;

		//Type of character
		CharacterType character_type;

		//The character's spawn position
		Position spawn;

		//The character's current direction
		Direction direction;

		//The character's current coordinate in the map
		Coordinate coordinate;

		//The character's movement speed (pixels per second)
		float move_speed;

		//The character's velocity
		Velocity velocity;

		//Move the character without checking for collision
		bool move_without_checking;

		//Is the character colliding with a wall in the direction specified?
		bool isColliding(const Direction & direction, Map & map);

		//Set the position of the remaining sprites if the character is an enemy
		virtual void setFrightenedPosition(const Position & position);

		//Move the frightened sprites
		virtual void moveFrightened();

		//Move exploding sprites
		virtual void moveExploding();

		//Set the position of the exploding sprites
		virtual void setExplodingPosition(const Position & position);

	public:

		//Default constructor
		Character();
		
		//Initialize the character
		//const Position & spawn_position: starting position of the character
		//const float & move_speed: the character's movement speed (pixels per second)
		//const std::string & file_path: the file path of the character's sprite sheet
		//Map & map: the map the character will move in
		void init(const Position & spawn_position, const std::string & file_path, Map & map);

		//Move the character (including collision and teleportation)
		void move(Map & map, const float & time_per_frame);

		//Teleport the character
		void teleport();

		//Alternate between the character's 2 animation states
		void playAnimation();

		//Set the character's direction
		void setDirection(const Direction & direction);

		//Get the character's current direction
		Direction getDirection() const;

		//Set the character's position
		void setPosition(const Position & position);

		//Get the character's current position
		Position getPosition() const;

		//Get the type of the character
		CharacterType getCharacterType() const;

		//Destructor
		~Character();

	};
}

#endif