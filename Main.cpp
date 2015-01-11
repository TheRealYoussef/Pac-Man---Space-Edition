#include "SFML/Graphics.hpp"
#include <iostream>
#include "Game.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pac Man - Space Edition");
	pac::ClockTime time_per_frame;
	pac::ClockTime FPS;
	int FPS_counter = 0;
	FPS.time = sf::seconds(1);
	FPS.clock.restart();
	pac::Game game;
	while (window.isOpen())
	{
		FPS_counter++;
		if (FPS.clock.getElapsedTime() >= FPS.time)
		{
			std::cout << FPS_counter << std::endl;
			FPS_counter = 0;
			FPS.clock.restart();
		}
		time_per_frame.time = sf::seconds(time_per_frame.clock.getElapsedTime().asSeconds());
		time_per_frame.clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			game.functionsInEventLoop(event, window, time_per_frame.time.asSeconds());
		}
		game.functionsInGameLoop(window, time_per_frame.time.asSeconds());
		window.clear(sf::Color(255, 255, 255));
		game.display(window);
		window.display();
	}
	return 0;
}