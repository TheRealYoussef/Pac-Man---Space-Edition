#include "Player.h"
#include "Enemy.h"

#ifndef Blinky_h
#define Blinky_h

namespace pac
{
	class Blinky : public Enemy
	{

	public:

		//Default constructor
		Blinky();

		//Target the player's position
		void targetPlayer(const Player & player);

	};
}

#endif