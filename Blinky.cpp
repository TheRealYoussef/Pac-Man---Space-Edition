#include <stdlib.h>
#include <stdio.h>
#include "Player.h"
#include "Blinky.h"
#include "GlobalVariables.h"

pac::Blinky::Blinky()
{
	direction = pac::LEFT;
	for (int i = 0; i < pac::NUMBER_OF_SAFETY_FRAMES; i++)
	{
		previous_directions[i] = direction;
	}
	mode = pac::SCATTER;
	get_out_of_ghost_house_points = -1;
}

void pac::Blinky::targetPosition(const pac::Player & player)
{
	if (mode == pac::CHASE)
	{
		target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
	}
}