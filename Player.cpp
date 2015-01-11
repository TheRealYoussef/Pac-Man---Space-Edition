#include "SFML/Graphics.hpp"
#include "Map.h"
#include "Point.h"
#include "Character.h"
#include "Player.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Player::Player()
{
}

void pac::Player::init(const pac::Position & spawn_position, const float & move_speed, const std::string & player_file_path, pac::Map & map)
{
	animation.time = player_animation_time;
	this->move_speed = move_speed;
	direction = pac::RIGHT;
	next_direction = pac::NONE;
	map.getTeleportationPairs(teleportation_pairs);
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		images[i].texture.loadFromFile(player_file_path, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
	}
	for (int i = 1; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		images[i].texture.loadFromFile(player_file_path, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
	}
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		images[i].sprite.setTexture(images[i].texture);
		images[i].sprite.setPosition(spawn_position.x, spawn_position.y);
	}
}

void pac::Player::chooseDirection(const sf::Event & event, sf::RenderWindow & window, const float & time_per_frame, pac::Map & map)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Right:
			setDirection(pac::RIGHT, time_per_frame, map);
			break;
		case sf::Keyboard::Left:
			setDirection(pac::LEFT, time_per_frame, map);
			break;
		case sf::Keyboard::Up:
			setDirection(pac::UP, time_per_frame, map);
			break;
		case sf::Keyboard::Down:
			setDirection(pac::DOWN, time_per_frame, map);
			break;
		}
	}
}

void pac::Player::setDirection(const pac::Direction & direction, const float & time_per_frame, pac::Map & map)
{
	if (!isColliding(direction, time_per_frame, map))
	{
		move_without_checking = true;
		next_direction = pac::NONE;
		this->direction = direction;
	}
	else
	{
		if (velocity != 0)
		{
			next_direction = direction;
		}
	}
}

void pac::Player::eatPoints(pac::Map & map)
{
	if (map.getPoint(coordinate, direction, point))
	{
		if ((*point).getEatenState() == pac::NOT_EATEN)
		{
			if (direction == pac::RIGHT || direction == pac::LEFT)
			{
				if (lessThanOrEqual(getPosition().x + pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().x + pac::TILE_SIZE.width - (*point).getPointDistance()) && biggerThanOrEqual(getPosition().x + pac::CHARACTER_SIZE.width - pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().x + (*point).getPointDistance()) && lessThan(getPosition().y, (*point).getPosition().y) && biggerThan(getPosition().y + pac::CHARACTER_SIZE.height, (*point).getPosition().y + pac::TILE_SIZE.height))
				{
					(*point).eat();
				}
			}
			else if (direction == pac::UP || direction == pac::DOWN)
			{
				if (lessThan(getPosition().x, (*point).getPosition().x) && biggerThan(getPosition().x + pac::CHARACTER_SIZE.width, (*point).getPosition().x + pac::TILE_SIZE.width) && lessThanOrEqual(getPosition().y + pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + pac::TILE_SIZE.height - (*point).getPointDistance()) && biggerThanOrEqual(getPosition().y + pac::CHARACTER_SIZE.height - pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + (*point).getPointDistance()))
				{
					(*point).eat();
				}
			}
		}
	}
}

void pac::Player::executeStoredDirection(const float & time_per_frame, pac::Map & map)
{
	if (next_direction != pac::NONE)
	{
		if (!isColliding(next_direction, time_per_frame, map))
		{
			this->direction = next_direction;
			next_direction = pac::NONE;
			move_without_checking = true;
		}
	}
}

pac::Player::~Player()
{
}