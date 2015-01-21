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

		//
		std::vector <Direction> previous_directions;

		//
		bool same_direction;

		//
		ClockTime scatter;

		//
		ClockTime chase;

		//
		ClockTime frightened;

		//
		bool forced_switch;

		//
		void calculateDistance(const Coordinate & coordinate, const Direction & direction, Map & map);

		//
		void compareWithCurrentDirection(const bool & same_direction);

		//
		void compareWithAnotherDirection(Map & map);

		//
		void getOtherDirection();
		
		//
		bool checkPreviousDirections(const Direction & direction);

		//
		bool checkPreviousDirectionsOpposites(const Direction & direction);

	protected:

		//
		Position target_position;

		//
		Position scatter_position;

		//
		GhostMode mode;

	public:

		//
		Enemy();

		//
		void setScatterPosition(const Map & map, const Coordinate & coordinate);

		//
		void changeMode();

		//
		void chooseBestDirection(Map & map);

	};
}
#endif