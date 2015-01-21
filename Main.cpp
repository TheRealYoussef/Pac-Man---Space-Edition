#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "GlobalVariables.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pac Man - Space Edition");
	pac::ClockTime FPS;
	sf::Clock time_per_frame_clock;
	float time_per_frame;
	int FPS_counter = 0;
	FPS.time = sf::seconds(1);
	FPS.clock.restart();
	pac::Game game;
	time_per_frame_clock.restart();
	while (window.isOpen())
	{
		if (time_per_frame_clock.getElapsedTime().asSeconds() >= pac::LOWEST_TIME_PER_FRAME)
		{
			time_per_frame = time_per_frame_clock.getElapsedTime().asSeconds();
			//printf("%f \n", time_per_frame);
			time_per_frame_clock.restart();
			FPS_counter++;
			if (FPS.clock.getElapsedTime() >= FPS.time)
			{
				//printf( "%u \n", FPS_counter);
				FPS_counter = 0;
				FPS.clock.restart();
			}
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				game.functionsInEventLoop(event, window);
			}
			game.functionsInGameLoop(window, time_per_frame);
			window.clear(sf::Color(255, 255, 255));
			game.display(window);
			window.display();
		}
	}
	return 0;
}