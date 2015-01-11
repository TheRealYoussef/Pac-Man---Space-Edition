#include "GlobalVariables.h"

#ifndef Point_h
#define Point_h

namespace pac
{
	class Point
	{
	private:

		//Point's position
		Position position;

		//Point's type (normal, power, etc.)
		PointType type;

		//How much does the point add to the score if eaten
		int value;

		//The point's sf::Sprite 
		sf::Sprite image;

		//Is the point eaten or not
		EatenState eaten;

		//The distance between the sprite boundaries and the actual image of the point
		float point_distance;

	public:

		//Default constructor
		Point();

		//Initialize the point
		//const Position & position: position of the point relative to the SFML window
		//const PointType & type: type of the point
		//const int & value: value of the point
		//sf::Texture & texture: texture of the point
		void init(const Position & position, const PointType & type, const int & value, sf::Texture & texture);

		//Get the point's position
		Position getPosition() const;

		//Get the point distance (the distance between the sprite boundaries and the actual image of the point)
		float getPointDistance() const;

		//Get the eaten state of the point (eaten or not)
		EatenState getEatenState() const;

		//Eat the point
		void eat();

		//Display the point
		void display(sf::RenderWindow & window) const;

		//Destructor
		~Point();

	};
}

#endif