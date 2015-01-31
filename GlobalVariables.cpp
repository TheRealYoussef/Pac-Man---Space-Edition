#include "SFML/Graphics.hpp"

#include "GlobalVariables.h"

pac::Position::Position()
{
}

pac::Position::Position(const float & x, const float & y)
{
	this->x = x;
	this->y = y;
}

pac::Position& pac::Position::operator=(const pac::Position & position)
{
	x = position.x; 
	y = position.y;
	return *this; 
}

pac::Position& pac::Position::operator=(const sf::Vector2f & position)
{ 
	x = position.x;
	y = position.y;
	return *this;
}

const pac::Position pac::Position::operator+(const pac::Position & position) const
{ 
	float x1 = x + position.x; 
	float y1 = y + position.y;
	return pac::Position(x1, y1);
}

bool pac::Position::operator!=(const Position & position) const
{
	return ((this->x != position.x) || (this->y != position.y));
}

pac::Size::Size()
{
}

pac::Size::Size(const float & width, const float & height) 
{ 
	this->width = width; 
	this->height = height; 
}

pac::Size& pac::Size::operator=(const pac::Size & size)
{ 
	width = size.width; 
	height = size.height; 
	return *this; 
}

pac::Scale::Scale()
{
}

pac::Scale::Scale(const float & x, const float & y)
{ 
	this->x = x; 
	this->y = y; 
}

pac::Scale& pac::Scale::operator = (const pac::Scale & scale)
{ 
	x = scale.x; 
	y = scale.y; 
	return *this; 
}

pac::Distance::Distance()
{
}

pac::Distance::Distance(const float & x, const float & y)
{ 
	this->x = x; 
	this->y = y; 
}

pac::Distance& pac::Distance::operator=(const pac::Distance & distance)
{ 
	x = distance.x; 
	y = distance.y; 
	return *this; 
}

pac::Image::Image()
{
}

pac::TileBox::TileBox()
{
}

pac::TileBox& pac::TileBox::operator=(const pac::TileBox & tile_box)
{ 
	position = tile_box.position; 
	wall = tile_box.wall; 
	return *this; 
}

pac::GridSize::GridSize()
{
}

pac::GridSize::GridSize(const int & row, const int & col)
{
	this->row = row; 
	this->col = col; 
}

pac::ClockTime::ClockTime()
{
}

pac::Velocity::Velocity()
{
}

pac::Velocity::Velocity(const float & x, const float & y) 
{ 
	this->x = x; 
	this->y = y; 
}

pac::Velocity& pac::Velocity::operator=(const pac::Velocity & velocity) 
{ 
	x = velocity.x; 
	y = velocity.y; 
	return *this; 
}

pac::Velocity& pac::Velocity::operator=(const float & value) 
{ 
	x = value; 
	y = value; 
	return *this; 
}

bool pac::Velocity::operator!=(const int & value) 
{ 
	return (x != value || y != value); 
}

pac::Coordinate::Coordinate()
{
}

pac::Coordinate::Coordinate(const int & x, const int & y) 
{ 
	this->x = x; 
	this->y = y; 
}

pac::Coordinate& pac::Coordinate::operator=(const pac::Coordinate & coordinate) 
{ 
	x = coordinate.x; 
	y = coordinate.y; 
	return *this; 
}

pac::TeleportationPairInfo::TeleportationPairInfo()
{
}

pac::TeleportationPairInfo::TeleportationPairInfo(const pac::Coordinate & teleporter_1_c, const pac::Direction & teleporter_1_d, const pac::Coordinate & teleporter_2_c, const pac::Direction & teleporter_2_d)
{ 
	this->teleporter_1_c = teleporter_1_c; 
	this->teleporter_1_d = teleporter_1_d; 
	this->teleporter_2_c = teleporter_2_c; 
	this->teleporter_2_d = teleporter_2_d; 
}