#include "Line.h"

Line::Line(const Point& A, const Point& B)
{
	pointA = A; pointB = B;
}

Line::Line(const sf::Vector2f& A, const sf::Vector2f& B)
{
	pointA.x = A.x;
	pointA.y = A.y;

	pointB.x = B.x;
	pointB.y = B.y;
}

Line::Line(const double& angleCoef, const double& move)
{
	//y = kx + b;
	pointA.x = 0;
	pointA.y = move;

	pointB.x = 1;
	pointB.y = angleCoef + move;
}
