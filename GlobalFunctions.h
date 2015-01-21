#include "GlobalVariables.h"

#ifndef GlobalFunctions_h
#define GlobalFunctions_h

//Is arg1 == arg2?
bool equal(const float & arg1, const float & arg2);

//Is arg1 != arg2?
bool notEqual(const float & arg1, const float & arg2);

//Is arg1 >= arg2?
bool biggerThanOrEqual(const float & arg1, const float & arg2);

//Is arg1 <= arg2?
bool lessThanOrEqual(const float & arg1, const float & arg2);

//Is arg1 > arg2?
bool biggerThan(const float & arg1, const float & arg2);

//Is arg1 < arg2?
bool lessThan(const float & arg1, const float & arg2);

//Opposite of Right is Left and opposite of Up is Down and vice versa
pac::Direction opposite(const pac::Direction & direction);

#endif