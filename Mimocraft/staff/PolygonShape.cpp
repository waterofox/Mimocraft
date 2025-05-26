#include "PolygonShape.h"
#define polygon (*this)


PolygonShape::PolygonShape(const std::vector<Point>& points) : Shape(points.size(),sf::LinesStrip)
{
	for (size_t i = 0; i < points.size(); ++i)
	{
		(*this)[i] = points[i];
	}
}

PolygonShape::PolygonShape(const std::initializer_list<Point>& points) : Shape(points.size(), sf::LinesStrip)
{
	for (size_t i = 0; i < points.size(); ++i)
	{
		(*this)[i] = *(points.begin() + i);
	}
}

const std::vector<Point> PolygonShape::getVertexs_s()
{
	std::vector<Point> resualt(getVertexCount());
	for (size_t i = 0; i < getVertexCount(); ++i)
	{
		resualt[i].x = (*this)[i].position.x;
		resualt[i].y = (*this)[i].position.y;
	}
	return resualt;
}

bool PolygonShape::isConvex()
{
	bool flag;
	for (long long i = 0; i < getVertexCount(); ++i)
	{
		const sf::Vector2f& A = polygon[myMod(i - 1, getVertexCount())].position;
		const sf::Vector2f& C = polygon[myMod(i + 1, getVertexCount())].position;

		sf::Vector2f AB = polygon[i].position - A;
		sf::Vector2f BC = C - polygon[i].position;

		double resualt = AB.x * BC.y - AB.y * BC.x;
		if (i == 0) { flag = resualt >= 0; continue; }
		if (flag != resualt >= 0) { return false; }


	}
	return true;
}
