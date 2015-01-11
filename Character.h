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

		//Set the character's velocity based on his direction and collision
		void setCharacterVelocity(const float & time_per_frame, Map & map);

	protected:

		//The character's current direction
		Direction direction;

		//The character's current coordinate in the map
		Coordinate coordinate;
	
		//The character's animation clock and time
		ClockTime animation;

		//The current state of the character's animation
		AnimationState animation_state;

		//The character's movement clock and time
		ClockTime movement;

		//The character's movement speed (pixels per second)
		float move_speed;

		//The character's velocity
		Velocity velocity;

		//The images of the differect sprites of the character
		Image images[NUMBER_OF_SPRITES_PER_CHARACTER];

		//Did the movement clock reach or exceed the movement time?
		bool moved;

		//Is the character colliding with a wall in the direction specified?
		bool isColliding(const Direction & direction, const float & time_per_frame, Map & map);

		//Move the character without checking for collision
		bool move_without_checking;

		//A vector of pointers to all the teleportation pairs in the map
		std::vector <TeleportationPair*> teleportation_pairs;

		//Set the velocity
		void setVelocity(const Velocity & velocity, const float & time_per_frame, Map & map);

	public:

		//Default constructor
		Character();

		//Move the character (including collision and teleportation)
		//For teleportation to work, initTele(pac::Map & map) has to be called 
		void move(Map & map, const float & time_per_frame);

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

		//Display the character
		void display(sf::RenderWindow & window) const;

		//Destructor
		~Character();

	};
}

#endif