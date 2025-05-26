#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

struct Point : public sf::Vector2f 
{
	Point(double x, double y) : sf::Vector2f(x, y) {}
	Point() : sf::Vector2f() {}
};

class Line
{
public:
	Point pointA;
	Point pointB;

	Line(const Point& A, const Point& B);
	Line(const sf::Vector2f& A, const sf::Vector2f& B);
	Line(const double& angleCoef, const double& move);

};

