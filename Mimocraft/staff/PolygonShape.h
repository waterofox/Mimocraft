#pragma once
#include "Shape.h"
#include <initializer_list>
class PolygonShape : public ::Shape
{
public:
	
	PolygonShape(const std::vector<Point>& points);
	PolygonShape(const std::initializer_list<Point>& points);
	PolygonShape() : ::Shape() {}
	~PolygonShape() {}

	const std::vector<Point> getVertexs_s();
	bool isConvex();

};

