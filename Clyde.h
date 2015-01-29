#include "Enemy.h"
#include "GlobalVariables.h"

#ifndef Clyde_h
#define Clyde_h

namespace pac
{
	class Player;

	class Clyde : public Enemy
	{

	private:

		//Distance from the player
		float distance_from_player;

	public:

		//Default constructor
		Clyde();

		//If chasing, and the distance from the player is bigger than the equivilant of 8 tiles, then Clyde targets the player's position
		//Otherwise, he targets the scatter position
		void targetPosition(const Player & player, const Map & map);

	};
}

#endif