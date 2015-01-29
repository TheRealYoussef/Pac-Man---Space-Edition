#include "Player.h"
#include "Pinky.h"
#include "GlobalVariables.h"

pac::Pinky::Pinky()
{
	direction = pac::DOWN;
	for (int i = 0; i < pac::NUMBER_OF_SAFETY_FRAMES; i++)
	{
		previous_directions[i] = direction;
	}
	mode = pac::HOUSE;
	get_out_of_ghost_house_points = pac::PINKY_GET_OUT_OF_GHOST_HOUSE_POINTS;
}

void pac::Pinky::targetPosition(const pac::Player & player)
{
	if (mode == pac::CHASE)
	{
		switch (player.getDirection())
		{
		case pac::RIGHT:
			target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f + pac::PINKY_TARGETING_DISTANCE * pac::TILE_SIZE.width, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
			break;
		case pac::LEFT:
			target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f - pac::PINKY_TARGETING_DISTANCE * pac::TILE_SIZE.width, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
			break;
		case pac::UP:
			target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f - pac::PINKY_TARGETING_DISTANCE * pac::TILE_SIZE.height);
			break;
		case pac::DOWN:
			target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f + pac::PINKY_TARGETING_DISTANCE * pac::TILE_SIZE.height);
			break;
		}
	}
}