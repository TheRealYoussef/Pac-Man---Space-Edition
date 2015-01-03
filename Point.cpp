#include "SFML/Graphics.hpp"

#include "Player.h"
#include "Point.h"
#include "GlobalVariables.h"

Point::Point()
{
	eaten = true;
}

Point::Point(const Position & position, const PointType & type, const int & value, sf::Texture & texture)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->type = type;
	this->value = value;
	image.texture = &texture;
	image.sprite.setTexture(*(image.texture));
	image.sprite.setScale(TILE_SCALE.x, TILE_SCALE.y);
	image.sprite.setPosition(position.x, position.y);
	eaten = false;
}

Point& Point::operator=(const Point & p)
{
	position.x = p.position.x;
	position.y = p.position.y;
	type = p.type;
	value = p.value;
	image.sprite.setTexture(*(p.image.texture));
	image.sprite.setScale(TILE_SCALE.x, TILE_SCALE.y);
	image.sprite.setPosition(position.x, position.y);
	eaten = false;
	return *this;
}

void Point::eat(const Player & player)
{
	if (!eaten)
	{
		if (player.getPosition().x + ACTUAL_EATING_DISTANCE < position.x + TILE_SIZE.width * TILE_SCALE.x && player.getPosition().x + CHARACTER_SIZE.width - ACTUAL_EATING_DISTANCE > position.x && player.getPosition().y < position.y && player.getPosition().y + CHARACTER_SIZE.height > position.y + TILE_SIZE.height * TILE_SCALE.y && (player.getDirection() == RIGHT || player.getDirection() == LEFT))
			eaten = true;
		else if (player.getPosition().x < position.x && player.getPosition().x + CHARACTER_SIZE.width > position.x + TILE_SIZE.width * TILE_SCALE.x && player.getPosition().y + ACTUAL_EATING_DISTANCE < position.y + TILE_SIZE.height * TILE_SCALE.y && player.getPosition().y + CHARACTER_SIZE.height - ACTUAL_EATING_DISTANCE > position.y + TILE_SIZE.height && (player.getDirection() == UP || player.getDirection() == DOWN))
			eaten = true;
	}
}

void Point::display(sf::RenderWindow & window) const
{
	if (!eaten)
		window.draw(image.sprite);
}
