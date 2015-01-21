#include "SFML/Graphics.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

#include "ButtonArray.h"
#include "GlobalVariables.h"

pac::ButtonArray::ButtonArray(const int & number_of_buttons, const pac::Distance & distance_between_buttons, const pac::Position & array_starting_position, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str()), selected_state_buttons_file(selected_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail() && !selected_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, pac::IMAGE);
		normal_state_images.resize(number_of_buttons);
		selected_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			readFile(selected_state_buttons_file, selected_state_images, i);
			setOriginAndPosition(normal_state_images, i);
			setOriginAndPosition(selected_state_images, i);
		}
	}
	else
	{
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
		fileFailure(selected_state_buttons_file, selected_state_buttons_file_path);
	}
}

pac::ButtonArray::ButtonArray(const int & number_of_buttons, const pac::Distance & distance_between_buttons, const pac::Position & array_starting_position, const pac::Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, pac::SCALE);
		setScaleInitializations(scale_amount, scale_time);
		normal_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			setOriginAndPosition(normal_state_images, i);
		}
	}
	else
	{
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
	}
}

pac::ButtonArray::ButtonArray(const int & number_of_buttons, const pac::Distance & distance_between_buttons, const pac::Position & array_starting_position, const pac::Scale & scale_amount, const sf::Time & scale_time, const std::string & normal_state_buttons_file_path, const std::string & selected_state_buttons_file_path)
{
	std::ifstream normal_state_buttons_file(normal_state_buttons_file_path.c_str()), selected_state_buttons_file(selected_state_buttons_file_path.c_str());
	if (!normal_state_buttons_file.fail() && !selected_state_buttons_file.fail())
	{
		setInitializations(number_of_buttons, distance_between_buttons, array_starting_position, pac::IMAGE_SCALE);
		setScaleInitializations(scale_amount, scale_time);
		normal_state_images.resize(number_of_buttons);
		selected_state_images.resize(number_of_buttons);
		for (int i = 0; i < number_of_buttons; i++)
		{
			readFile(normal_state_buttons_file, normal_state_images, i);
			readFile(selected_state_buttons_file, selected_state_images, i);
			setOriginAndPosition(normal_state_images, i);
			setOriginAndPosition(selected_state_images, i);
		}
	}
	else
	{
		fileFailure(normal_state_buttons_file, normal_state_buttons_file_path);
		fileFailure(selected_state_buttons_file, selected_state_buttons_file_path);
	}
}

void pac::ButtonArray::setInitializations(const int & number_of_buttons, const pac::Distance & distance_between_buttons, const pac::Position & array_starting_position, const pac::TransitionType & transition_type)
{
	this->number_of_buttons = number_of_buttons;
	this->distance_between_buttons = distance_between_buttons;
	this->array_starting_position = array_starting_position;
	this->transition = transition_type;
	selected_button_index = -1;
	error_message_displayed = false;
	number_of_functions.resize(number_of_buttons);
	for (int i = 0; i < number_of_buttons; i++)
	{
		number_of_functions[i] = 0;
	}
	functions.resize(number_of_buttons);
}

void pac::ButtonArray::setScaleInitializations(const pac::Scale & scale_amount, const sf::Time & scale_time)
{
	this->scale_amount = scale_amount;
	this->scale.time = scale_time;
	scale_bool = false;
}

void pac::ButtonArray::readFile(std::ifstream & file, std::vector <pac::Image> & images, const int & index)
{
	std::string file_path;
	std::getline(file, file_path);
	if (!images[index].texture.loadFromFile(file_path))
	{
		std::cerr << "Failed to load " << file_path << std::endl;
	}
	images[index].sprite.setTexture(images[index].texture);
}

void pac::ButtonArray::setOriginAndPosition(std::vector <pac::Image> & images, const int & index)
{
	float total_heights_of_buttons_above = 0;
	for (int j = 0; j < index; j++)
	{
		total_heights_of_buttons_above += images[j].sprite.getLocalBounds().height;
	}
	images[index].sprite.setOrigin(sf::Vector2f(images[index].sprite.getLocalBounds().width / 2, images[index].sprite.getLocalBounds().height / 2));
	images[index].sprite.setPosition(sf::Vector2f(array_starting_position.x + index * distance_between_buttons.x, array_starting_position.y + index * distance_between_buttons.y + total_heights_of_buttons_above));
}

void pac::ButtonArray::fileFailure(std::ifstream & file, const std::string & file_path)
{
	if (file.fail())
	{
		std::cerr << "Failed to open " << file_path << std::endl;
	}
}

void pac::ButtonArray::setState(const sf::Event & event, sf::RenderWindow & window)
{
	mouseMoved(event, window);
	mouseButtonPressed(event, window);
	upKeyPressed(event);
	downKeyPressed(event);
	returnKeyPressed(event, window);
}

void pac::ButtonArray::mouseMoved(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::MouseMoved)
	{
		if (mouseOnButton(event, window))
		{
			changeScales();
		}
	}
}

void pac::ButtonArray::mouseButtonPressed(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (mouseOnSelectedButton(event, window))
		{
			callFunctions(window);
		}
	}
}

void pac::ButtonArray::upKeyPressed(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			previously_selected_button_index = selected_button_index;
			selected_button_index--;
			if (selected_button_index <= -1)
			{
				selected_button_index = number_of_buttons - 1;
			}
			changeScales();
		}
	}
}

void pac::ButtonArray::downKeyPressed(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Down)
		{
			previously_selected_button_index = selected_button_index;
			selected_button_index++;
			if (selected_button_index == number_of_buttons)
			{
				selected_button_index = 0;
			}
			changeScales();
		}
	}
}

void pac::ButtonArray::returnKeyPressed(const sf::Event & event, sf::RenderWindow & window)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Return)
		{
			callFunctions(window);
		}
	}
}

bool pac::ButtonArray::mouseOnButton(const sf::Event & event, sf::RenderWindow & window)
{
	for (int i = 0; i < number_of_buttons; i++)
	{
		if (sf::Mouse::getPosition(window).x >= normal_state_images[i].sprite.getPosition().x - normal_state_images[i].sprite.getLocalBounds().width / 2.f && sf::Mouse::getPosition(window).x <= normal_state_images[i].sprite.getPosition().x + normal_state_images[i].sprite.getLocalBounds().width / 2.f && sf::Mouse::getPosition(window).y >= normal_state_images[i].sprite.getPosition().y - normal_state_images[i].sprite.getLocalBounds().height / 2.f && sf::Mouse::getPosition(window).y <= normal_state_images[i].sprite.getPosition().y + normal_state_images[i].sprite.getLocalBounds().height / 2.f)
		{
			if (i == selected_button_index)
			{
				return false;
			}
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

bool pac::ButtonArray::mouseOnSelectedButton(const sf::Event & event, sf::RenderWindow & window)
{
	if (selected_button_index != -1)
	{
		return (sf::Mouse::getPosition(window).x >= normal_state_images[selected_button_index].sprite.getPosition().x - normal_state_images[selected_button_index].sprite.getLocalBounds().width / 2.f && sf::Mouse::getPosition(window).x <= normal_state_images[selected_button_index].sprite.getPosition().x + normal_state_images[selected_button_index].sprite.getLocalBounds().width / 2.f && sf::Mouse::getPosition(window).y >= normal_state_images[selected_button_index].sprite.getPosition().y - normal_state_images[selected_button_index].sprite.getLocalBounds().height / 2.f && sf::Mouse::getPosition(window).y <= normal_state_images[selected_button_index].sprite.getPosition().y + normal_state_images[selected_button_index].sprite.getLocalBounds().height / 2.f);
	}
	return false;
}

void pac::ButtonArray::callFunctions(sf::RenderWindow & window)
{
	if (selected_button_index != -1)
	{
		for (unsigned int i = 0; i < number_of_functions[selected_button_index]; i++)
		{
			if (functions[selected_button_index][i] != NULL)
			{
				functions[selected_button_index][i]();
			}
		}
	}
}

void pac::ButtonArray::changeScales()
{
	if (transition == pac::SCALE || transition == pac::IMAGE_SCALE)
	{
		scale_bool = true;
		scale.clock.restart();
		if (previously_selected_button_index != -1)
		{
			if (transition == pac::SCALE)
			{
				normal_state_images[previously_selected_button_index].sprite.setScale(sf::Vector2f(1.0, 1.0));
			}
			else if (transition == pac::IMAGE_SCALE)
			{
				selected_state_images[previously_selected_button_index].sprite.setScale(sf::Vector2f(1.0, 1.0));
			}
		}
	}
}

void pac::ButtonArray::scaleButton(std::vector <pac::Image> & images)
{
	if (selected_button_index != -1)
	{
		if (scale_bool)
		{
			double m_x = pow(scale_amount.x, 1.f / scale.time.asSeconds());
			double m_y = pow(scale_amount.y, 1.f / scale.time.asSeconds());
			images[selected_button_index].sprite.setScale(pow(m_x, scale.clock.getElapsedTime().asSeconds()), pow(m_y, scale.clock.getElapsedTime().asSeconds()));
		}
		if (scale.clock.getElapsedTime() >= scale.time)
		{
			scale_bool = false;
			images[selected_button_index].sprite.setScale(scale_amount.x, scale_amount.y);
		}
	}
}

void pac::ButtonArray::scaleSelectedButton()
{
	if (transition == pac::SCALE)
	{
		scaleButton(normal_state_images);
	}
	else if (transition == pac::IMAGE_SCALE)
	{
		scaleButton(selected_state_images);
	}
	else
	{
		if (!error_message_displayed)
		{
			std::cout << "Can't scale buttons when the transition type is set to IMAGE. Use another button array constructor which sets a scale amount and a scale time to be able to use this function." << std::endl;
			error_message_displayed = true;
		}
	}
}

void pac::ButtonArray::setFunction(const int & button_index, void(*function)())
{
	functions[button_index].push_back(function);
	number_of_functions[button_index]++;
}

void pac::ButtonArray::display(sf::RenderWindow & window) const
{
	for (int i = 0; i < number_of_buttons; i++)
	{
		if (transition == pac::IMAGE || transition == pac::IMAGE_SCALE)
		{
			if (i != selected_button_index)
			{
				window.draw(normal_state_images[i].sprite);
			}
		}
		else
		{
			window.draw(normal_state_images[i].sprite);
		}
	}
	if (transition == pac::IMAGE || transition == pac::IMAGE_SCALE)
	{
		if (selected_button_index != -1)
		{
			window.draw(selected_state_images[selected_button_index].sprite);
		}
	}
}

pac::ButtonArray::~ButtonArray()
{
}