#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include "ButtonArray.h"
#include "player.h"
#include "Map.h"
#include "GlobalVariables.h"

void output()
{
	std::cout << "WORKS" << std::endl;
}

void output2()
{
	std::cout << "ALSO WORKS" << std::endl;
}

void close(sf::RenderWindow & window)
{
	window.close();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pac Man - Space Edition");
	window.setKeyRepeatEnabled(false);
	Player X(Position{ 400.0f - 15.0f, (600.0f - 496.0f) / 2.0f + 16.0f * 22.0f + 8.0f }, sf::seconds(0.2f), 175.0f, "Assets/Sprites/Pac Man.png");
	Map M(Position{ 145, ((600 - 496) / 2.0) }, "Assets/Maps/Pac Man Map.txt");
	//ButtonArray button_test(3, Distance{ 70, 20 }, Position{ 350, 200 }, Scale{ 1.3, 1.2 }, sf::seconds(0.3), "Assets/Files/Normal Buttons File Paths.txt");
	//ButtonArray button_test(3, Distance{ 70, 20 }, Position{ 350, 200 }, "Assets/Files/Normal Buttons File Paths.txt", "Assets/Files/Selected Buttons File Paths.txt");
	//ButtonArray button_test(3, Distance{ 70, 20 }, Position{ 350, 200 }, Scale{ 1.3, 1.2 }, sf::seconds(0.3), "Assets/Files/Normal Buttons File Paths.txt", "Assets/Files/Selected Buttons File Paths.txt");
	/*button_test.setFunction(0, &output);
	button_test.setFunction(1, &output2);
	button_test.setFunction(2, &close);*/
	ClockTime FPS;
	int FPS_counter = 0;
	FPS.time = sf::seconds(1);
	FPS.clock.restart();
	while (window.isOpen())
	{
		X.playAnimation();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			X.initializeMovement(event, M);
			//button_test.setState(event, window);
		}
		X.move(M);
		M.eatPoint(X);
		//button_test.scaleSelectedButton();
		window.clear(sf::Color(255, 255, 255));
		M.display(window);
		M.displayPoints(window);
		X.display(window);
		M.displayTeleportationTiles(window);
		//button_test.display(window);
		window.display();
		FPS_counter++;
		if (FPS.clock.getElapsedTime() >= FPS.time)
		{
			std::cout << FPS_counter << std::endl;
			FPS_counter = 0;
			FPS.clock.restart();
		}
	}
	return 0;
}