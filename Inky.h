#include "Blinky.h"
#include "Enemy.h"
#include "GlobalVariables.h"

#ifndef Inky_h
#define Inky_h

namespace pac
{
	class Player;

	class Inky : public Enemy
	{

	private:

		//The position of the center point of the tile which is 2 tiles infront of the player (in the direction he is moving)
		Position two_tiles_infront_of_player;

		//The distance between the position mentioned above and the blinky's center point position
		Distance tile_and_blinky_distance;

	public:

		//Default constructor
		Inky();

		//Target double tile_and_blinky_distance position if chasing
		void targetPosition(const Player & player, const Blinky & blinky);

	};
}

#endif