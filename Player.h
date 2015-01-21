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
		void setDirection(const Direction & direction, Map & map);

	public:

		//Default constructor
		Player();

		//User clicks to choose the next direction
		void chooseDirection(const sf::Event & event, sf::RenderWindow & window, Map & map);

		//Execute the stored direction command (if not used, then the user will have to be exact if he wants to change his direction)
		void executeStoredDirection(Map & map);

		//Player eats the points on the map
		void eatPoints(Map & map);

		//Destructor
		~Player();

	};
}

#endif