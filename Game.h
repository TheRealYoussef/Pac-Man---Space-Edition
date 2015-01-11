#include "Map.h"
#include "Player.h"

#include "GlobalVariables.h"

#ifndef Game_h
#define Game_h

namespace pac
{
	class Game
	{

	private:

		//
		Map map;

		//
		Player player;

		//
		ClockTime frame;

	public:

		//
		Game();

		//
		float getTimePerFrame();

		//
		void functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window, const float & time_per_frame);

		//
		void functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame);

		//
		void display(sf::RenderWindow & window);

		//Destructor
		~Game();

	};
}

#endif