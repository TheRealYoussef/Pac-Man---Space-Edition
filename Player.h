#include "SFML/Graphics.hpp"

#include "Map.h"
#include "Point.h"
#include "Character.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

#include "GlobalVariables.h"

#ifndef Player_h
#define Player_h

namespace pac
{
	class Player : public Character
	{

	private:

		//The exploding sprite sheet of the player
		Image exploding_images[NUMBER_OF_SPRITES_PER_CHARACTER];

		//The explosion animation states
		AnimationState exploding_animation_state;

		//The clock that controls the change of animation state
		sf::Clock animation_state_clock;

		//The clock that controls the length of the animation state
		sf::Clock animation_length_clock;

		//Is the player alive
		bool alive;

		//The player's next direction
		Direction next_direction;

		//Pointer to a point
		Point *point;

		//The player's number of lives remaining
		int number_of_lives;

		//Set the player's direction
		void setDirection(const Direction & direction, Map & map);

		//Frighten the ghosts
		void frightenGhosts(Blinky & blinky, Pinky & pinky, Inky & inky, Clyde & clyde);

		//True if collided with enemy
		bool collidedEnemy(Enemy & enemy);

		//Move exploding sprites
		void moveExploding();

		//The the position of the exploding sprites
		void setExplodingPosition(const Position & position);

		//Kill the player
		void killPlayer();

	public:

		//Default constructor
		Player();

		//Load exploding image
		void loadExplodingImage(const std::string & exploding_file_path);

		//Play exploding animation
		void playExplodingAnimation();

		//User clicks to choose the next direction
		void chooseDirection(const sf::Event & event, sf::RenderWindow & window, Map & map);

		//Execute the stored direction command (if not used, then the user will have to be exact if he wants to change his direction)
		void executeStoredDirection(Map & map);

		//Player eats the points on the map
		void eatPoints(Map & map, Blinky & blinky, Pinky & pinky, Inky & inky, Clyde & clyde);

		//Handle all ghost collision
		void handleGhostCollision(Blinky & blinky, Pinky & pinky, Inky & inky, Clyde & clyde);

		//Display the player
		void display(sf::RenderWindow & window) const;

		//Is the player alive
		bool isAlive() const;

		//Destructor
		~Player();

	};
}

#endif