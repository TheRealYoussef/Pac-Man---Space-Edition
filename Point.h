#include "GlobalVariables.h"

#ifndef Point_h
#define Point_h

class Player;

class Point
{
private:

	//
	Position position;

	//
	PointType type;

	//
	int value;

	//
	TextureSprite image;

	//
	bool eaten;

public:

	//
	Point();

	//
	Point(const Position & position, const PointType & type, const int & value, sf::Texture & texture);

	//
	Point& operator=(const Point & p);

	//
	void eat(const Player & player);

	//
	void display(sf::RenderWindow & window) const;

};

#endif
