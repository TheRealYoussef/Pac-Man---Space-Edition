#include "Player.h"
#include "Blinky.h"
#include "Enemy.h"
#include "GlobalVariables.h"

#ifndef Inky_h
#define Inky_h

namespace pac
{
	class Inky : public Enemy
	{

	private:

		//
		Position two_tiles_infront_of_player;

		//
		Distance tile_and_blinky_distance;

	public:

		//
		Inky();

		//
		void targetDoubleBlinkyPlayerDistance(const Player & player, const Blinky & blinky);

	};
}

#endif