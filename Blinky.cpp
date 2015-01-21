#include <stdlib.h>
#include <stdio.h>
#include "Player.h"
#include "Blinky.h"
#include "GlobalVariables.h"

pac::Blinky::Blinky()
{
	direction = LEFT;
}

void pac::Blinky::targetPlayer(const pac::Player & player)
{
	if (mode == pac::CHASE)
	{
		target_position = pac::Position(player.getPosition().x + pac::CHARACTER_SIZE.width / 2.f, player.getPosition().y + pac::CHARACTER_SIZE.height / 2.f);
	}
}