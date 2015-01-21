#include <iostream>
#include <cmath>

#include "Character.h"
#include "TeleportationPair.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"


pac::TeleportationPair::TeleportationPair()
{
}

void pac::TeleportationPair::init(const pac::Position & teleportation_1_p, const pac::Direction & teleportation_1_d, const pac::Position & teleportation_2_p, const pac::Direction & teleportation_2_d)
{
	this->teleportation_1_p = teleportation_1_p;
	this->teleportation_1_d = teleportation_1_d;
	this->teleportation_2_p = teleportation_2_p;
	this->teleportation_2_d = teleportation_2_d;
}

pac::TeleportationPair & pac::TeleportationPair::operator=(const pac::TeleportationPair & tp)
{
	teleportation_1_p = tp.teleportation_1_p;
	teleportation_1_d = tp.teleportation_1_d;
	teleportation_2_p = tp.teleportation_2_p;
	teleportation_2_d = tp.teleportation_2_d;
	return *this;
}

void pac::TeleportationPair::teleport(pac::Character & character) const
{
	switchEnter(character, teleportation_1_d, teleportation_1_p, teleportation_2_d, teleportation_2_p);
	switchEnter(character, teleportation_2_d, teleportation_2_p, teleportation_1_d, teleportation_1_p);
}

void pac::TeleportationPair::switchEnter(pac::Character & character, const pac::Direction & direction_1, const pac::Position & position_1, const pac::Direction & direction_2, const pac::Position & position_2) const
{
	switch (direction_1)
	{
	case RIGHT:
		if (biggerThanOrEqual(character.getPosition().x, position_1.x) && equal(round(character.getPosition().y), position_1.y - pac::TILE_BOX.height) && character.getDirection() == pac::RIGHT)
			switchLeave(character, direction_2, position_2);
		break;
	case LEFT:
		if (lessThanOrEqual(character.getPosition().x, position_1.x) && equal(round(character.getPosition().y), position_1.y - pac::TILE_BOX.height) && character.getDirection() == pac::LEFT)
			switchLeave(character, direction_2, position_2);
		break;
	case UP:
		if (equal(round(character.getPosition().x), position_1.x - pac::TILE_BOX.width) && lessThanOrEqual(character.getPosition().y, position_1.y) && character.getDirection() == pac::UP)
			switchLeave(character, direction_2, position_2);
		break;
	case DOWN:
		if (equal(round(character.getPosition().x), position_1.x - pac::TILE_BOX.width) && biggerThanOrEqual(character.getPosition().y, position_1.y) && character.getDirection() == pac::DOWN)
			switchLeave(character, direction_2, position_2);
		break;
	}
}

void pac::TeleportationPair::switchLeave(pac::Character & character, const pac::Direction & direction, const pac::Position & position) const
{
	switch (direction)
	{
	case pac::RIGHT:
		character.setPosition(pac::Position( position.x, position.y - pac::TILE_BOX.height ));
		character.setDirection(opposite(direction));
		character.dontCheckCollision();
		break;
	case pac::LEFT:
		character.setPosition(pac::Position( position.x, position.y - pac::TILE_BOX.height ));
		character.setDirection(opposite(direction));
		character.dontCheckCollision();
		break;
	case pac::UP:
		character.setPosition(pac::Position( position.x - pac::TILE_BOX.width, position.y ));
		character.setDirection(opposite(direction));
		character.dontCheckCollision();
		break;
	case pac::DOWN:
		character.setPosition(pac::Position( position.x - pac::TILE_BOX.width, position.y ));
		character.setDirection(opposite(direction));
		character.dontCheckCollision();
		break;
	}
}

void pac::TeleportationPair::getTeleporterPositions(pac::Position position_pair[pac::PAIR]) const
{
	position_pair[0] = teleportation_1_p;
	position_pair[1] = teleportation_2_p;
}

void pac::TeleportationPair::getTeleporterDirections(pac::Direction direction_pair[pac::PAIR]) const
{
	direction_pair[0] = teleportation_1_d;
	direction_pair[1] = teleportation_2_d;
}

pac::TeleportationPair::~TeleportationPair()
{
}