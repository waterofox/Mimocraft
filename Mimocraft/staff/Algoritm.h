#pragma once
#include <SFML/Graphics.hpp>
#define PI 3.1415926535

static sf::Vector2f rotateIN(const sf::Vector2f& cords)
{
	double radians = 45 * PI / 180;
	sf::Vector2f newPosition;
	newPosition.x = cos(radians) * cords.x - sin(radians) * cords.y;
	newPosition.y = sin(radians) * cords.x + cos(radians) * cords.y;
	return newPosition;
}
static sf::Vector2f rotateOUT(const sf::Vector2f& cords)
{
	double radians = -45 * PI / 180;
	sf::Vector2f newPosition;
	newPosition.x = cos(radians) * cords.x - sin(radians) * cords.y;
	newPosition.y = sin(radians) * cords.x + cos(radians) * cords.y;
	return newPosition;
}
static int myMod(const int& a, const int& b) { return (a % b + b) % b; }

static sf::Vector2i absVector(sf::Vector2i chunk)
{
	chunk.x = abs(chunk.x);
	chunk.y = abs(chunk.y);
	return chunk;
}

static std::string unite(const sf::Vector2i& cords)
{
	return std::to_string(cords.x) + '_' + std::to_string(cords.y);
}