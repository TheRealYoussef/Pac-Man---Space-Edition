#include "SFML/Graphics.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

#include "ButtonArray.h"
#include "GlobalVariables.h"

ButtonArray::ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str()), selected_state_buttons_file(selected_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail() && !selected_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, IMAGE);
		normal_state_images.resize(number_of_buttons);
		selected_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			readFile(selected_state_buttons_file, selected_state_images, i);
			setOriginAndPosition(normal_state_images, i);
			setOriginAndPosition(selected_state_images, i);
			fillFunctionsNull(i);
		}
	}
	else
	{
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
		fileFailure(selected_state_buttons_file, selected_state_buttons_file_path);
	}
}

ButtonArray::ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, SCALE);
		setScaleInitializations(scale_amount, scale_time);
		normal_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			setOriginAndPosition(normal_state_images, i);
			fillFunctionsNull(i);
		}
	}
	else
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
}

ButtonArray::ButtonArray(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str()), selected_state_buttons_file(selected_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail() && !selected_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, IMAGE_SCALE);
		setScaleInitializations(scale_amount, scale_time);
		normal_state_images.resize(number_of_buttons);
		selected_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			readFile(selected_state_buttons_file, selected_state_images, i);
			setOriginAndPosition(normal_state_images, i);
			setOriginAndPosition(selected_state_images, i);
			fillFunctionsNull(i);
		}
	}
	else
	{
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
		fileFailure(selected_state_buttons_file, selected_state_buttons_file_path);
	}
}

void ButtonArray::setInitializations(const int & number_of_buttons, const Distance & distance_between_buttons, const Position & array_starting_position, const TransitionType & transition_type)
{
	this->number_of_buttons = number_of_buttons;
	this->distance_between_buttons = distance_between_buttons;
	this->array_starting_position = array_starting_position;
	this->transition = transition_type;
	selected_button_index = -1;
	error_message_displayed = false;
	functions_0.resize(number_of_buttons);
	functions_1.resize(number_of_buttons);
}

void ButtonArray::setScaleInitializations(const Scale & scale_amount, const sf::Time & scale_time)
{
	this->scale_amount = scale_amount;
	this->scale.time = scale_time;
	scale_bool = false;
}

void ButtonArray::readFile(std::ifstream & file, std::vector <TextureSprite> & images, const int & index)
{
	std::string file_path;
	std::getline(file, file_path);
	images[index].texture = new sf::Texture;
	if (!images[index].texture->loadFromFile(file_path))
		std::cerr << "Failed to load " << file_path << std::endl;
	images[index].sprite.setTexture(*images[index].texture);
}

void ButtonArray::setOriginAndPosition(std::vector <TextureSprite> & images, const int & index)
{
	float total_heights_of_buttons_above = 0;
	for (int j = 0; j < index; j++)
		total_heights_of_buttons_above += images[j].sprite.getLocalBounds().height;
	images[index].sprite.setOrigin(sf::Vector2f(images[index].sprite.getLocalBounds().width / 2, images[index].sprite.getLocalBounds().height / 2));
	images[index].sprite.setPosition(sf::Vector2f(array_starting_position.x + index * distance_between_buttons.x, array_starting_position.y + index * distance_between_buttons.y + total_heights_of_buttons_above));
}

void ButtonArray::fillFunctionsNull(const int & index)
{
	functions_0[index].push_back(NULL);
	functions_1[index].push_back(NULL);
}

void ButtonArray::fileFailure(std::ifstream & file, const std::string & file_path)
{
	if (file.fail())
		std::cerr << "Failed to open " << file_path << std::endl;
}

void ButtonArray::setState(const sf::Event & event, sf::RenderWindow & window)
{
	mouseMoved(event, window);
	mouseButtonPressed(event, window);
	upKeyPressed(event);
	downKeyPressed(event);
	returnKeyPressed(event, window);
}

void ButtonArray::mouseMoved(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::MouseMoved)
		if (mouseOnButton(event, window))
			changeScales();
}

void ButtonArray::mouseButtonPressed(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::MouseButtonPressed)
		if (mouseOnSelectedButton(event, window))
			callFunctions(window);
}

void ButtonArray::upKeyPressed(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			previously_selected_button_index = selected_button_index;
			selected_button_index--;
			if (selected_button_index <= -1)
				selected_button_index = number_of_buttons - 1;
			changeScales();
		}
	}
}

void ButtonArray::downKeyPressed(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Down)
		{
			previously_selected_button_index = selected_button_index;
			selected_button_index++;
			if (selected_button_index == number_of_buttons)
				selected_button_index = 0;
			changeScales();
		}
	}
}

void ButtonArray::returnKeyPressed(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::KeyPressed)
		if (event.key.code == sf::Keyboard::Return)
			callFunctions(window);
}

bool ButtonArray::mouseOnButton(const sf::Event & event, sf::RenderWindow & window)
{
	for (int i = 0; i < number_of_buttons; i++)
	{
		if (sf::Mouse::getPosition(window).x >= normal_state_images[i].sprite.getPosition().x - normal_state_images[i].sprite.getLocalBounds().width / 2.0 && sf::Mouse::getPosition(window).x <= normal_state_images[i].sprite.getPosition().x + normal_state_images[i].sprite.getLocalBounds().width / 2.0 && sf::Mouse::getPosition(window).y >= normal_state_images[i].sprite.getPosition().y - normal_state_images[i].sprite.getLocalBounds().height / 2.0 && sf::Mouse::getPosition(window).y <= normal_state_images[i].sprite.getPosition().y + normal_state_images[i].sprite.getLocalBounds().height / 2.0)
		{
			if (i == selected_button_index)
				return false;
			else
			{
				previously_selected_button_index = selected_button_index;
				selected_button_index = i;
				return true;
			}
		}
	}
	return false;
}

bool ButtonArray::mouseOnSelectedButton(const sf::Event & event, sf::RenderWindow & window)
{
	if (selected_button_index != -1)
		return (sf::Mouse::getPosition(window).x >= normal_state_images[selected_button_index].sprite.getPosition().x - normal_state_images[selected_button_index].sprite.getLocalBounds().width / 2.0 && sf::Mouse::getPosition(window).x <= normal_state_images[selected_button_index].sprite.getPosition().x + normal_state_images[selected_button_index].sprite.getLocalBounds().width / 2.0 && sf::Mouse::getPosition(window).y >= normal_state_images[selected_button_index].sprite.getPosition().y - normal_state_images[selected_button_index].sprite.getLocalBounds().height / 2.0 && sf::Mouse::getPosition(window).y <= normal_state_images[selected_button_index].sprite.getPosition().y + normal_state_images[selected_button_index].sprite.getLocalBounds().height / 2.0);
	return false;
}

void ButtonArray::callFunctions(sf::RenderWindow & window)
{
	if (selected_button_index != -1)
	{
		if (functions_0[selected_button_index][0] != NULL)
		{
			for (unsigned int i = 0; i < functions_0[selected_button_index].size(); i++)
				functions_0[selected_button_index][i]();
		}
		if (functions_1[selected_button_index][0] != NULL)
		{
			for (unsigned int i = 0; i < functions_1[selected_button_index].size(); i++)
				functions_1[selected_button_index][i](window);
		}
	}
}

void ButtonArray::changeScales()
{
	if (transition == SCALE || transition == IMAGE_SCALE)
	{
		scale_bool = true;
		scale.clock.restart();
		if (previously_selected_button_index != -1)
		{
			if (transition == SCALE)
				normal_state_images[previously_selected_button_index].sprite.setScale(sf::Vector2f(1.0, 1.0));
			else if (transition == IMAGE_SCALE)
				selected_state_images[previously_selected_button_index].sprite.setScale(sf::Vector2f(1.0, 1.0));
		}
	}
}

void ButtonArray::scaleButton(std::vector <TextureSprite> & images)
{
	if (selected_button_index != -1)
	{
		if (scale_bool)
		{
			double m_x = pow(scale_amount.x, 1.0 / scale.time.asSeconds());
			double m_y = pow(scale_amount.y, 1.0 / scale.time.asSeconds());
			images[selected_button_index].sprite.setScale(pow(m_x, scale.clock.getElapsedTime().asSeconds()), pow(m_y, scale.clock.getElapsedTime().asSeconds()));
		}
		if (scale.clock.getElapsedTime() >= scale.time)
		{
			scale_bool = false;
			images[selected_button_index].sprite.setScale(scale_amount.x, scale_amount.y);
		}
	}
}

void ButtonArray::scaleSelectedButton()
{
	if (transition == SCALE)
		scaleButton(normal_state_images);
	else if (transition == IMAGE_SCALE)
		scaleButton(selected_state_images);
	else
	{
		if (!error_message_displayed)
		{
			std::cout << "Can't scale buttons when the transition type is set to IMAGE. Use another button array constructor which sets a scale amount and a scale time to be able to use this function." << std::endl;
			error_message_displayed = true;
		}
	}
}

void ButtonArray::setFunction(const int & button_index, void (*function)())
{
	if (functions_0[button_index][0] == NULL)
		functions_0[button_index][0] = function;
	else
		functions_0[button_index].push_back(function);
}

void ButtonArray::setFunction(const int & button_index, void(*function)(sf::RenderWindow & window))
{
	if (functions_1[button_index][0] == NULL)
		functions_1[button_index][0] = function;
	else
		functions_1[button_index].push_back(function);
}

void ButtonArray::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < number_of_buttons; i++)
	{
		if (transition == IMAGE || transition == IMAGE_SCALE)
		{
			if (i != selected_button_index)
				window.draw(normal_state_images[i].sprite);
		}
		else
			window.draw(normal_state_images[i].sprite);
	}
	if (transition == IMAGE || transition == IMAGE_SCALE)
	{
		if (selected_button_index != -1)
			window.draw(selected_state_images[selected_button_index].sprite);
	}
}
