#include "Player.h"
#include "Blinky.h"
#include "Inky.h"
#include "GlobalVariables.h"

pac::Inky::Inky()
{
	direction = pac::UP;
	for (int i = 0; i < pac::NUMBER_OF_SAFETY_FRAMES; i++)
	{
		previous_directions[i] = direction;
	}
	mode = pac::HOUSE;
	get_out_of_ghost_house_points = pac::INKY_GET_OUT_OF_GHOST_HOUSE_POINTS;
}

void pac::Inky::targetPosition(const pac::Player & player, const pac::Blinky & blinky)
{
	if (mode == pac::CHASE)
	{
		switch (player.getDirection())
		{
		case pac::RIGHT:
			two_tiles_infront_of_player = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f + 2 * pac::TILE_SIZE.width, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
			break;
		case pac::LEFT:
			two_tiles_infront_of_player = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f - 2 * pac::TILE_SIZE.width, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
			break;
		case pac::UP:
			two_tiles_infront_of_player = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f - 2 * pac::TILE_SIZE.height);
			break;
		case pac::DOWN:
			two_tiles_infront_of_player = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f + 2 * pac::TILE_SIZE.height);
			break;
		}

		tile_and_blinky_distance = pac::Distance(two_tiles_infront_of_player.x - (blinky.getPosition().x + pac::CHARACTER_SIZE.width / 2.f), two_tiles_infront_of_player.y - (blinky.getPosition().y + pac::CHARACTER_SIZE.height / 2.f));

		target_position = pac::Position(two_tiles_infront_of_player.x + tile_and_blinky_distance.x, two_tiles_infront_of_player.y + tile_and_blinky_distance.y);
	}
}