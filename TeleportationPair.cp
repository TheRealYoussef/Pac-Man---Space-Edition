#include <iostream>
#include <cmath>

#include "Player.h"
#include "TeleportationPair.h"
#include "GlobalVariables.h"

TeleportationPair::TeleportationPair(const Position & teleportation_1_p, const Direction & teleportation_1_d, const Position & teleportation_2_p, const Direction & teleportation_2_d)
{
	this->teleportation_1_p.x = teleportation_1_p.x;
	this->teleportation_1_p.y = teleportation_1_p.y;
	this->teleportation_1_d = teleportation_1_d;
	this->teleportation_2_p.x = teleportation_2_p.x;
	this->teleportation_2_p.y = teleportation_2_p.y;
	this->teleportation_2_d = teleportation_2_d;
}

TeleportationPair & TeleportationPair::operator=(const TeleportationPair & tp)
{
	teleportation_1_p.x = tp.teleportation_1_p.x;
	teleportation_1_p.y = tp.teleportation_1_p.y;
	teleportation_1_d = tp.teleportation_1_d;
	teleportation_2_p.x = tp.teleportation_2_p.x;
	teleportation_2_p.y = tp.teleportation_2_p.y;
	teleportation_2_d = tp.teleportation_2_d;
	return *this;
}

void TeleportationPair::teleport(Player & object) const
{
	switchEnter(object, teleportation_1_d, teleportation_1_p, teleportation_2_d, teleportation_2_p);
	switchEnter(object, teleportation_2_d, teleportation_2_p, teleportation_1_d, teleportation_1_p);
}

void TeleportationPair::switchEnter(Player & object, const Direction & direction_1, const Position & position_1, const Direction & direction_2, const Position & position_2) const
{
	switch (direction_1)
	{
	case RIGHT:
		if (object.getPosition().x >= position_1.x && round(object.getPosition().y) == position_1.y - TILE_BOX.height)
			switchLeave(object, direction_2, position_2);
		break;
	case LEFT:
		if (object.getPosition().x <= position_1.x && round(object.getPosition().y) == position_1.y - TILE_BOX.height)
			switchLeave(object, direction_2, position_2);
		break;
	case UP:
		if (round(object.getPosition().x) == position_1.x - TILE_BOX.width && object.getPosition().y <= position_1.y)
			switchLeave(object, direction_2, position_2);
		break;
	case DOWN:
		if (round(object.getPosition().x) == position_1.x - TILE_BOX.width && object.getPosition().y >= position_1.y)
			switchLeave(object, direction_2, position_2);
		break;
	}
}

void TeleportationPair::switchLeave(Player & object, const Direction & direction, const Position & position) const
{
	switch (direction)
	{
	case RIGHT:
		object.setPosition(Position{ position.x - 1, position.y - TILE_BOX.height });
		object.setDirection(LEFT);
		break;
	case LEFT:
		object.setPosition(Position{ position.x + 1, position.y - TILE_BOX.height });
		object.setDirection(RIGHT);
		break;
	case UP:
		object.setPosition(Position{ position.x - TILE_BOX.width, position.y + 1 });
		object.setDirection(DOWN);
		break;
	case DOWN:
		object.setPosition(Position{ position.x - TILE_BOX.width, position.y - 1 });
		object.setDirection(UP);
		break;
	}
}
