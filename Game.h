#include "Map.h"
#include "Player.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

#include "GlobalVariables.h"

#ifndef Game_h
#define Game_h

namespace pac
{
	class Game
	{

	private:

		Map *map;

		Player *player;

		Blinky *blinky;

		Pinky *pinky;

		Inky *inky;

		Clyde *clyde;

		sf::Clock ghost_animation_clock;

		sf::Clock player_animation_clock;

		sf::Clock frightened_animation_clock;

		void loadMap();

		void loadPlayer();

		void loadBlinky();

		void loadPinky();

		void loadInky();

		void loadClyde();

		void restartClocks();

		void playerFunctions(const float & time_per_frame);

		void enemiesAnimations();

		void enemiesFrightenedAnimations();

		void enemyFunctions(Enemy *enemy, const float & time_per_frame);

		void inkyFunctions(const float & time_per_frame);

	public:

		//Default constructor
		Game();

		//All functions in the event loop
		void functionsInEventLoop(const sf::Event & sf_event, sf::RenderWindow & window);

		//All functions in the game loop (outside the event loop)
		void functionsInGameLoop(sf::RenderWindow & window, const float & time_per_frame);

		//All the displays
		void display(sf::RenderWindow & window);

		//Destructor
		~Game();

	};
}

#endif