#include "Enemy.h"

#ifndef Blinky_h
#define Blinky_h

namespace pac
{
	class Player;

	class Blinky : public Enemy
	{

	public:

		//Default constructor
		Blinky();

		//Target the player's position if chasing
		void targetPosition(const Player & player);

	};
}

#endif