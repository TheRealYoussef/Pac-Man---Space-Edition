#include "Player.h"
#include "Enemy.h"

#ifndef Pinky_h
#define Pinky_h

namespace pac
{
	class Pinky : public Enemy
	{

	public:

		//Default constructor
		Pinky();

		//Target 4 tiles infront of the player
		void targetInfrontOfPlayer(const Player & player);

	};
}

#endif