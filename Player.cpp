#include "SFML/Graphics.hpp"
#include "Map.h"
#include "Point.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "Player.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Player::Player()
{
	character_type = pac::PLAYER;
	next_direction = pac::NONE;
	direction = pac::RIGHT;
	move_speed = pac::PLAYER_MOVE_SPEED;
	alive = true;
}

void pac::Player::loadExplodingImage(const std::string & exploding_file_path)
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		exploding_images[i].texture.loadFromFile(exploding_file_path, sf::IntRect(0.f, pac::CHARACTER_SIZE.width / 2 * i, pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.height));
	}
	for (int i = 1; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i += 2)
	{
		exploding_images[i].texture.loadFromFile(exploding_file_path, sf::IntRect(pac::CHARACTER_SIZE.width, pac::CHARACTER_SIZE.width / 2 * i - pac::CHARACTER_SIZE.width / 2, pac::CHARACTER_SIZE.height, pac::CHARACTER_SIZE.height));
	}
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		exploding_images[i].sprite.setTexture(exploding_images[i].texture);
		exploding_images[i].sprite.setPosition(getPosition().x, getPosition().y);
	}
}

void pac::Player::playExplodingAnimation()
{
	if (!alive)
	{
		if (animation_length_clock.getElapsedTime() <= pac::DURATION_OF_DEATH)
		{
			if (animation_state_clock.getElapsedTime() >= pac::PLAYER_ANIMATION_TIME)
			{
				exploding_animation_state = (exploding_animation_state == pac::STATE_1) ? pac::STATE_2 : pac::STATE_1;
				animation_state_clock.restart();
			}
		}
		else
		{
			exploding_animation_state = pac::STATE_1;
			//alive = true;
		}
	}
}

void pac::Player::chooseDirection(const sf::Event & event, sf::RenderWindow & window, pac::Map & map)
{
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Right:
			if (alive)
			{
				setDirection(pac::RIGHT, map);
			}
			break;
		case sf::Keyboard::Left:
			if (alive)
			{
				setDirection(pac::LEFT, map);
			}
			break;
		case sf::Keyboard::Up:
			if (alive)
			{
				setDirection(pac::UP, map);
			}
			break;
		case sf::Keyboard::Down:
			if (alive)
			{
				setDirection(pac::DOWN, map);
			}
			break;
		}
	}
}

void pac::Player::setDirection(const pac::Direction & direction, pac::Map & map)
{
	if (!isColliding(direction, map))
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

void pac::Player::eatPoints(pac::Map & map, pac::Blinky & blinky, pac::Pinky & pinky, pac::Inky & inky, pac::Clyde & clyde)
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
					map.decrementPoints();
					if ((*point).getPointType() == pac::POWER_POINT)
					{
						frightenGhosts(blinky, pinky, inky, clyde);
					}
				}
			}
			else if (direction == pac::UP || direction == pac::DOWN)
			{
				if (lessThan(getPosition().x, (*point).getPosition().x) && biggerThan(getPosition().x + pac::CHARACTER_SIZE.width, (*point).getPosition().x + pac::TILE_SIZE.width) && lessThanOrEqual(getPosition().y + pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + pac::TILE_SIZE.height - (*point).getPointDistance()) && biggerThanOrEqual(getPosition().y + pac::CHARACTER_SIZE.height - pac::ACTUAL_EATING_DISTANCE, (*point).getPosition().y + (*point).getPointDistance()))
				{
					(*point).eat();
					map.decrementPoints();
					if ((*point).getPointType() == pac::POWER_POINT)
					{
						frightenGhosts(blinky, pinky, inky, clyde);
					}
				}
			}
		}
	}
}

void pac::Player::frightenGhosts(pac::Blinky & blinky, pac::Pinky & pinky, pac::Inky & inky, pac::Clyde & clyde)
{
	blinky.frighten();
	pinky.frighten();
	inky.frighten();
	clyde.frighten();
}


void pac::Player::executeStoredDirection(pac::Map & map)
{
	if (next_direction != pac::NONE)
	{
		if (!isColliding(next_direction, map))
		{
			this->direction = next_direction;
			next_direction = pac::NONE;
			move_without_checking = true;
		}
	}
}

void pac::Player::handleGhostCollision(pac::Blinky & blinky, pac::Pinky & pinky, pac::Inky & inky, pac::Clyde & clyde)
{
	if (collidedEnemy(blinky))
	{
		if (blinky.getMode() == pac::FRIGHTENED)
		{
			blinky.returnHouse();
		}
		else if (blinky.getMode() == pac::CHASE || blinky.getMode() == pac::SCATTER || blinky.getMode() == pac::LEAVE_HOUSE)
		{
			killPlayer();
		}
	}
	if (collidedEnemy(pinky))
	{
		if (pinky.getMode() == pac::FRIGHTENED)
		{
			pinky.returnHouse();
		}
		else if (pinky.getMode() == pac::CHASE || pinky.getMode() == pac::SCATTER || pinky.getMode() == pac::LEAVE_HOUSE)
		{
			killPlayer();
		}
	}
	if (collidedEnemy(inky))
	{
		if (inky.getMode() == pac::FRIGHTENED)
		{
			inky.returnHouse();
		}
		else if (inky.getMode() == pac::CHASE || inky.getMode() == pac::SCATTER || inky.getMode() == pac::LEAVE_HOUSE)
		{
			killPlayer();
		}
	}
	if (collidedEnemy(clyde))
	{
		if (clyde.getMode() == pac::FRIGHTENED)
		{
			clyde.returnHouse();
		}
		else if (clyde.getMode() == pac::CHASE || clyde.getMode() == pac::SCATTER || clyde.getMode() == pac::LEAVE_HOUSE)
		{
			killPlayer();
		}
	}
}

bool pac::Player::collidedEnemy(pac::Enemy & enemy)
{
	if (lessThan(getPosition().x + pac::CHARACTER_SIZE.width / 2.f + pac::TILE_SIZE.width / 2.f, enemy.getPosition().x + pac::CHARACTER_SIZE.width / 2.f - pac::TILE_SIZE.width / 2.f) || biggerThan(getPosition().x + pac::CHARACTER_SIZE.width / 2.f - pac::TILE_SIZE.width / 2.f, enemy.getPosition().x + pac::CHARACTER_SIZE.width / 2.f + pac::TILE_SIZE.width / 2.f) || lessThan(getPosition().y + pac::CHARACTER_SIZE.height / 2.f + pac::TILE_SIZE.height / 2.f, enemy.getPosition().y + pac::CHARACTER_SIZE.height / 2.f - pac::TILE_SIZE.height / 2.f) || biggerThan(getPosition().y + pac::CHARACTER_SIZE.height / 2.f - pac::TILE_SIZE.width / 2.f, enemy.getPosition().y + pac::CHARACTER_SIZE.height / 2.f + pac::TILE_SIZE.height / 2.f))
	{
		return false;
	}
	return true;
}

void pac::Player::display(sf::RenderWindow & window) const
{
	if (exploding_animation_state == pac::STATE_1)
	{
		window.draw(images[(int)direction * 2 + (int)animation_state].sprite);
	}
	else
	{
		window.draw(exploding_images[(int)direction * 2 + (int)animation_state].sprite);
	}
}

bool pac::Player::isAlive() const
{
	return alive;
}

pac::Player::~Player()
{
}

void pac::Player::moveExploding()
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		exploding_images[i].sprite.move(velocity.x, velocity.y);
	}
}

void pac::Player::killPlayer()
{
	if (alive)
	{
		alive = false;
		number_of_lives--;
		animation_state_clock.restart();
		animation_length_clock.restart();
	}
}

void pac::Player::setExplodingPosition(const pac::Position & position)
{
	for (int i = 0; i < pac::NUMBER_OF_SPRITES_PER_CHARACTER; i++)
	{
		exploding_images[i].sprite.setPosition(position.x, position.y);
	}
}