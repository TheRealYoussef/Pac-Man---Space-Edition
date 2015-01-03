#include "SFML/Graphics.hpp"

#include <iostream>

#include "Map.h"
#include "Player.h"
#include "GlobalVariables.h"

Player::Player(const Position & spawn_position, const sf::Time & animation_time, const float & move_speed, const std::string & player_file_path)
{
	setInitializations(spawn_position, animation_time, move_speed);
	loadTextures(player_file_path);
	setSprites();
}

void Player::setInitializations(const Position & spawn_position, const sf::Time & animation_time, const float & move_speed)
{
	spawn.x = spawn_position.x;
	spawn.y = spawn_position.y;
	animation.time = animation_time;
	move_speed_const = move_speed;
	moved = true;
	started_moving = false;
	animation_state = STATE_1;
	player_direction = RIGHT;
	next_direction = NONE;
	animation.clock.restart();
}

void Player::loadTextures(const std::string & player_file_path)
{
	for (int i = 0; i < 8; i += 2)
	{
		image[i].texture = new sf::Texture;
		image[i].texture->loadFromFile(player_file_path, sf::IntRect(0, 16 * i, 32, 32));
	}
	for (int i = 1; i < 8; i += 2)
	{
		image[i].texture = new sf::Texture;
		image[i].texture->loadFromFile(player_file_path, sf::IntRect(32, 16 * i - 16, 32, 32));
	}
}

void Player::setSprites()
{
	for (int i = 0; i < 8; i++)
	{
		image[i].sprite.setTexture(*(image[i].texture));
		image[i].sprite.setPosition(spawn.x, spawn.y);
	}
}

void Player::playAnimation()
{
	if (animation.clock.getElapsedTime() >= animation.time)
	{
		animation_state = (animation_state == STATE_1) ? STATE_2 : STATE_1;
		animation.clock.restart();
	}
}

void Player::display(sf::RenderWindow & window) const
{
	window.draw(image[player_direction * 2 + animation_state].sprite);
}

void Player::initializeMovement(const sf::Event & event, const Map & map)
{
	initialMovement(event);
	pressToChooseDirection(event, map);
}

void Player::initialMovement(const sf::Event & event)
{
	if (!started_moving)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)
			{
				started_moving = true;
				if (event.key.code == sf::Keyboard::Left)
					player_direction = LEFT;
				movement.clock.restart();
			}
		}
	}
}

void Player::pressToChooseDirection(const sf::Event & event, const Map & map)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Right:
			if (!map.isCollidingRight(*this))
			{
				next_direction = NONE;
				player_direction = RIGHT;
			}
			else
			{
				if (velocity.x != 0 || velocity.y != 0)
					next_direction = RIGHT;
			}
			break;
		case sf::Keyboard::Left:
			if (!map.isCollidingLeft(*this))
			{
				next_direction = NONE;
				player_direction = LEFT;
			}
			else
			{
				if (velocity.x != 0 || velocity.y != 0)
					next_direction = LEFT;
			}
			break;
		case sf::Keyboard::Up:
			if (!map.isCollidingUp(*this))
			{
				next_direction = NONE;
				player_direction = UP;
			}
			else
			{
				if (velocity.x != 0 || velocity.y != 0)
					next_direction = UP;
			}
			break;
		case sf::Keyboard::Down:
			if (!map.isCollidingDown(*this))
			{
				next_direction = NONE;
				player_direction = DOWN;
			}
			else
			{
				if (velocity.x != 0 || velocity.y != 0)
					next_direction = DOWN;
			}
		}
	}
}

void Player::move(const Map & map)
{
	if (started_moving)
	{	
		setPlayerDirection(map);
		map.teleport(*this);
		setMoveTimeAndSpeed();
		setPlayerVelocity(map);
		movePlayer();
	}
}

void Player::setPlayerDirection(const Map & map)
{
	switch (next_direction)
	{
	case RIGHT:
		if (!map.isCollidingRight(*this))
			player_direction = RIGHT;
		break;
	case LEFT:
		if (!map.isCollidingLeft(*this))
			player_direction = LEFT;
		break;
	case UP:
		if (!map.isCollidingUp(*this))
			player_direction = UP;
		break;
	case DOWN:
		if (!map.isCollidingDown(*this))
			player_direction = DOWN;
		break;
	}
}

void Player::setMoveTimeAndSpeed()
{
	if (moved)
	{
		moved = false;
		time_per_frame = movement.clock.getElapsedTime().asSeconds();
		movement.time = sf::seconds(time_per_frame);
		move_speed = move_speed_const * time_per_frame;
	}
}

void Player::setPlayerVelocity(const Map & map)
{
	switch (player_direction)
	{
	case RIGHT:
		if (next_direction == RIGHT)
		{
			velocity.x = move_speed;
			velocity.y = 0;
			next_direction = NONE;
		}
		else if (!map.isCollidingRight(*this))
		{
			velocity.x = move_speed;
			velocity.y = 0;
		}
		else
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		break;
	case LEFT:
		if (next_direction == LEFT)
		{
			velocity.x = -move_speed;
			velocity.y = 0;
			next_direction = NONE;
		}
		else if (!map.isCollidingLeft(*this))
		{
			velocity.x = -move_speed;
			velocity.y = 0;
		}
		else
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		break;
	case UP:
		if (next_direction == UP)
		{
			velocity.x = 0;
			velocity.y = -move_speed;
			next_direction = NONE;
		}
		else if (!map.isCollidingUp(*this))
		{
			velocity.x = 0;
			velocity.y = -move_speed;
		}
		else
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		break;
	case DOWN:
		if (next_direction == DOWN)
		{
			velocity.x = 0;
			velocity.y = 0;
			next_direction = NONE;
		}
		else if (!map.isCollidingDown(*this))
		{
			velocity.x = 0;
			velocity.y = move_speed;
		}
		else
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		break;
	}
}

void Player::movePlayer()
{
	if (movement.clock.getElapsedTime() >= movement.time)
	{
		for (int i = 0; i < 8; i++)
			image[i].sprite.move(velocity.x, velocity.y);
		movement.clock.restart();
		moved = true;
	}
}

void Player::setPosition(const Position & position)
{
	for (int i = 0; i < 8; i++)
		image[i].sprite.setPosition(position.x, position.y);
}

void Player::setDirection(const Direction & direction)
{
	player_direction = direction;
}

Position Player::getPosition() const
{
	return Position{ image[0].sprite.getPosition().x, image[0].sprite.getPosition().y };
}

float Player::getTimePerFrame() const
{
	return time_per_frame;
}

Direction Player::getDirection() const
{
	return player_direction;
}
