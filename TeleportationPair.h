#include "GlobalVariables.h"

#ifndef TeleportationPair_h
#define TeleportationPair_h

namespace pac
{

	class Character;

	class TeleportationPair
	{
	private:

		//Teleporter 1 position
		Position teleportation_1_p;

		//Teleporter 2 position
		Position teleportation_2_p;

		//Teleporter 1 direction of entry
		Direction teleportation_1_d;

		//Teleporter 2 direction of entry
		Direction teleportation_2_d;

		//Controls entry into a teleporter
		void switchEnter(Character & character, const Direction & direction_1, const Position & position_1, const Direction & direction_2, const Position & position_2) const;

		//Controls exit from a teleporter
		void switchLeave(Character & character, const Direction & direction, const Position & position) const;

		//Set position
		void setPosition(Character & character, const Position & position, const Direction & direction) const;

	public:

		//Default constructor
		TeleportationPair();

		//Initialize the teleportation pair
		//const Position & teleportation_1_p: teleporter 1 position
		//const Direction & teleportation_1_d: teleporter 1 direction
		//const Position & teleportation_2_p: teleporter 2 position
		//const Direction & teleportation_2_d: teleporter 2 direction
		void init(const Position & teleportation_1_p, const Direction & teleportation_1_d, const Position & teleportation_2_p, const Direction & teleportation_2_d);

		//Copy a teleporter pair
		TeleportationPair & operator=(const TeleportationPair & tp);

		//Teleport the object from 1 teleporter to the other
		void teleport(Character & character) const;

		//Get the positions of both teleporters
		void getTeleporterPositions(Position position_pair[PAIR]) const;

		//Get the entry directions of both teleporters
		void getTeleporterDirections(Direction direction_pair[PAIR]) const;

		//Destructor
		~TeleportationPair();

	};
}

#endif