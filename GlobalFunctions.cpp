#include "GlobalFunctions.h"

bool equal(const float & arg1, const float & arg2)
{
	return arg1 == arg2;
}

bool notEqual(const float & arg1, const float & arg2)
{
	return arg1 != arg2;
}

bool biggerThanOrEqual(const float & arg1, const float & arg2)
{
	return arg1 >= arg2;
}

bool lessThanOrEqual(const float & arg1, const float & arg2)
{
	return arg1 <= arg2;
}

bool biggerThan(const float & arg1, const float & arg2)
{
	return arg1 > arg2;
}

bool lessThan(const float & arg1, const float & arg2)
{
	return arg1 < arg2;
}

pac::Direction opposite(const pac::Direction & direction)
{
	switch (direction)
	{
	case pac::RIGHT:
		return pac::LEFT;
	case pac::LEFT:
		return pac::RIGHT;
	case pac::UP:
		return pac::DOWN;
	case pac::DOWN:
		return pac::UP;
	}
}