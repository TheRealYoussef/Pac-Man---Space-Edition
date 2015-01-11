#include "SFML/Graphics.hpp"

#include "Map.h"
#include "Point.h"
#include "Character.h"

#include "GlobalVariables.h"

#ifndef Player_h
#define Player_h

namespace pac
{
	class Player : public Character
	{

	private:

		//The player's next direction
		Direction next_direction;

		//Pointer to a point
		Point *point;

		//The player's number of lives remaining
		int number_of_lives;

		//Set the player's direction
		void setDirection(const Direction & direction, const float & time_per_frame, pac::Map & map);

	public:

		//Default constructor
		Player();

		//Initialize the player
		//const Position & spawn_position: starting position of the player
		//const float & move_speed: the player's movement speed (pixels per second)
		//const std::string & player_file_path: the file path of the player's sprite sheet
		//Map & map: the map the player will move in
		void init(const Position & spawn_position, const float & move_speed, const std::string & player_file_path, Map & map);

		//User clicks to choose the next direction
		void chooseDirection(const sf::Event & event, sf::RenderWindow & window, const float & time_per_frame, Map & map);

		//Execute the stored direction command (if not used, then the user will have to be exact if he wants to change his direction)
		void executeStoredDirection(const float & time_per_frame, Map & map);

		//Player eats the points on the map
		void eatPoints(Map & map);

		//Destructor
		~Player();

	};
}

#endif