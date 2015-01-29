#include "Enemy.h"

#ifndef Pinky_h
#define Pinky_h

namespace pac
{
	class Player;

	class Pinky : public Enemy
	{

	public:

		//Default constructor
		Pinky();

		//Target 4 tiles infront of the player if chasing
		void targetPosition(const Player & player);

	};
}

#endif