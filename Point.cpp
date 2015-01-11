#include "SFML/Graphics.hpp"

#include "Point.h"
#include "GlobalVariables.h"
#include "GlobalFunctions.h"

pac::Point::Point()
{
	eaten = pac::NOT_EATEN;
	type = pac::NO_POINT;
}

void pac::Point::init(const pac::Position & position, const pac::PointType & type, const int & value, sf::Texture & texture)
{
	this->position = position;
	this->type = type;
	this->value = value;
	image.setTexture(texture);
	image.setScale(TILE_SCALE.x, TILE_SCALE.y);
	image.setPosition(position.x, position.y);
}

pac::Position pac::Point::getPosition() const
{
	return position;
}

float pac::Point::getPointDistance() const
{
	return (type == pac::NORMAL_POINT) ? NORMAL_POINT_EATING_DISTANCE : 0.0f;
}

pac::EatenState pac::Point::getEatenState() const
{
	return eaten;
}

void pac::Point::eat()
{
	eaten = pac::EATEN;
}

void pac::Point::display(sf::RenderWindow & window) const
{
	if (eaten == pac::NOT_EATEN && type != pac::NO_POINT)
	{
		window.draw(image);
	}
}

pac::Point::~Point()
{
}