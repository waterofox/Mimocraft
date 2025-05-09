#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#define PI 3.1415926535

static sf::Vector2f rotate(const sf::Vector2f& cords, const double& angle)
{
	double radians = angle * PI / 180;
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

static std::string unite(const sf::Vector2i& cords) //create chunk string name
{
	return std::to_string(cords.x) + '_' + std::to_string(cords.y);
}