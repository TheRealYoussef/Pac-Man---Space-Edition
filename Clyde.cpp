#include <cmath>

#include "Player.h"
#include "Clyde.h"
#include "GlobalVariables.h"

pac::Clyde::Clyde()
{
	direction = LEFT;
}

void pac::Clyde::targetPlayerOrScatterTile(const pac::Player & player, const pac::Map & map)
{
	if (mode == pac::CHASE)
	{
		distance_from_player = sqrt(pow(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f - (getPosition().x + pac::CHARACTER_SIZE.width / 2.f), 2) + pow(player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f - (getPosition().y + pac::CHARACTER_SIZE.height / 2.f), 2));

		if (distance_from_player > pac::CLYDE_MAXIMUM_DISTANCE_FROM_PLAYER * pac::TILE_SIDE_SIZE)
		{
			target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
		}
		else
		{
			target_position = scatter_position;
		}
	}
}