#include "HelperFunctions.hpp"
#include <cmath>

float getLengthOfVector2f(sf::Vector2f vec)
{
	return std::sqrtf(std::powf(vec.x, 2) + std::powf(vec.y, 2));
}