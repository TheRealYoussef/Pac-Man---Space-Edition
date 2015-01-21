#include "Player.h"
#include "Enemy.h"

#ifndef Pinky_h
#define Pinky_h

namespace pac
{
	class Pinky : public Enemy
	{

	public:

		//
		Pinky();

		//
		void targetInfrontOfPlayer(const Player & player);

	};
}

#endif