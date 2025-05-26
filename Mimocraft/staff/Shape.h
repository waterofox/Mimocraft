#pragma once

#include "Line.h"
#include <algorithm>


class Shape : public sf::VertexArray 
{
private:
	size_t vertex_sSize;
	sf::Vector2f position;
public:
	Shape(const size_t&  vertex_sCount, const sf::PrimitiveType& type);
	Shape() : VertexArray() {}
	~Shape() {}

	//manipulation;
	void rotate(const float& angle); //+
	void reflex(const Point& center); //+
	void reflex(const Line& axis); //+
	void gomotetia(const Point& point, float coef); //No homo +

	bool operator==(const Shape& anotherShape);

	//getters;

	sf::Vector2f getPosition();

	float perimetr(); //+
	bool isCongruentTo(const Shape& another);
	bool isSimilarTo(const Shape& another); //true or false, coef //+
	bool containsPoint(const Point& point); //+
	float area() const ;

	sf::Vector2f vert(int pos) const 
	{
		return (*this)[myMod(pos,vertex_sSize)].position;
	}

	//setters;

	void setColor(const sf::Color& color); //+
	void setPosition(const sf::Vector2f& newPosition); //+

	float myMod(float a, float b) const 
	{
		return std::fmod(std::fmod(a, b) + b, b);
	}


};

