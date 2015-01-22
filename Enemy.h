#include <vector>

#include "Character.h"
#include "Map.h"
#include "Player.h"
#include "GlobalVariables.h"

#ifndef Enemy_h
#define Enemy_h

namespace pac
{
	class Enemy : public Character
	{
	private:

		//The coordinate of the enemy's center point
		Coordinate center_coordinate;

		//The distances between the centers 4 tiles around the enemy and the target position
		std::vector <float> distances;

		//The pac::NUMBER_OF_SAFETY_FRAMES previous directions of the enemy
		std::vector <Direction> previous_directions;

		//Did the enemy change its direction?
		bool same_direction;

		//Scatter mode clock
		sf::Clock scatter_clock;

		//Chase mode clock
		sf::Clock chase_clock;

		//Frightened mode clock
		sf::Clock frightened_clock;

		//Is the enemy forced to move in the opposite direction?
		bool forced_switch;

		//If the tile is not a wall tile, then calculate the distance from its center to the target position and store that distance in the distances vector
		//Otherwise, store -1
		void calculateDistance(const Coordinate & coordinate, const Direction & direction, Map & map);

		//Compare distances between surrounding tiles and target position with that of the tile in the enemy's current direction
		void compareWithCurrentDirection(const bool & same_direction);

		//If another direction wasn't chosen, then compare distances between surrounding tiles and target position with that of another tile
		void compareWithAnotherDirection(Map & map);

		//Choose the other direction
		void getOtherDirection();
		
		//Return true if the direction passed to the function is the same as any of the previous directions, false otherwise
		bool checkPreviousDirections(const Direction & direction);

		//Return true if the direction passed to the function is the opposite of any of the previous directions, false otherwise
		bool checkPreviousDirectionsOpposites(const Direction & direction);

	protected:

		//The enemy's target position
		Position target_position;

		//The enemy's scatter position
		Position scatter_position;

		//The enemy's current state (scatter, chase, or frightened)
		GhostMode mode;

	public:

		//Default constructor
		Enemy();

		//Set the enemy's scatter position
		void setScatterPosition(const Map & map, const Coordinate & coordinate);

		//Control the enemy's current mode/state
		void changeMode();

		//Choose the next direction to take
		void chooseBestDirection(Map & map);

	};
}
#endif