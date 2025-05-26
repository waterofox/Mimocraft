#include "Shape.h"

#define shape (*this)
#define PI 3.1415926535
 

Shape::Shape(const size_t& vertex_sCount, const sf::PrimitiveType& type) : VertexArray(type, vertex_sCount)
{
	shape.vertex_sSize = vertex_sCount;
	position = shape.getBounds().getPosition();
}

sf::Vector2f Shape::getPosition()
{
	return position;
}

float Shape::perimetr()
{
	if (vertex_sSize == 0) { return 0 ; }
	float resualt = 0;
	for (size_t i = 0; i < vertex_sSize - 1; ++i)
	{
		sf::Vector2f&  A = shape[i].position;
		sf::Vector2f& B = shape[i+1].position;

		resualt += (sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2)));
	}

	sf::Vector2f& A = shape[vertex_sSize-1].position;
	sf::Vector2f& B = shape[0].position;

	resualt += (sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2)));
	return resualt;
}

bool Shape::isCongruentTo(const Shape& another)
{
	float eps = 0.000001;
	if (shape.isSimilarTo(another))
	{
		if (abs(shape.area() - another.area()) <= eps) { return true; }
	}
}
float dist(const sf::Vector2f& first, const sf::Vector2f& second) 
{
	return sqrt(pow((first.x - second.x), 2) + pow((first.y - second.y), 2));
}
bool equal(float first, float second) 
{

	const float epsilon  = 0.0001;
	return abs(first - second) < epsilon;
}


bool Shape::isSimilarTo(const Shape & another)
{
	const float eps = 0.0001f;

	const Shape& casted = another;

	if (casted.getVertexCount() != shape.getVertexCount()) { return false; }

	bool flag = false;
	int count = shape.getVertexCount();
	if (count == 1) {
		return true;
	}
	for (int k = 1; k > -2; k -= 2) {
		for (int i = 0; i < count; ++i) {
			flag = true;
			double coe = dist(casted.vert((i + 1) * k), casted.vert(i * k));
			coe /= dist(vert(1), vert(0));
			for (int j = 0; j < count; ++j) 
			{
				sf::Vector2f v1 = vert(j) - vert(j + 1);
				sf::Vector2f v2 = vert(j + 2) - vert(j + 1);
				double vectorMulty = v1.x * v2.x + v1.y * v2.y;
				double v_cos = vectorMulty / (sqrt(v1.x * v1.x + v1.y * v1.y) * sqrt(v2.x * v2.x + v2.y * v2.y));

				sf::Vector2f w1 = casted.vert((i + j) * k) - casted.vert((i + j + 1) * k);
				sf::Vector2f w2 = casted.vert((i + j + 2) * k) - casted.vert((i + j + 1) * k);
				vectorMulty = w1.x * w2.x + w1.y * w2.y;
				double w_cos = vectorMulty / (sqrt(w1.x * w1.x + w1.y * w1.y) * sqrt(w2.x * w2.x + w2.y * w2.y));

				if (!(equal(dist(casted.vert((i + j) * k),casted.vert((i + j + 1) * k)), coe * dist(vert(j), vert(j + 1))) && equal(v_cos, w_cos))) 
				{
					flag = false;
					break;
				}
			}
			if (flag) {
				return true;
			}
		}
	}
	return false;
}

bool Shape::containsPoint(const Point& point)
{
	if (vertex_sSize < 3) return false; 

	bool inside = false;
	size_t j = vertex_sSize - 1; 

	for (size_t i = 0; i < vertex_sSize; j = i++) {
		const sf::Vector2f& p1 = shape[i].position;
		const sf::Vector2f& p2 = shape[j].position;
		bool yInRange = (p1.y > point.y) != (p2.y > point.y);

		if (yInRange) {
			float intersectX = (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x;
			if (point.x < intersectX) {
				inside = !inside; 
			}
		}
	}

	return inside;
}

float Shape::area() const
{
	float sum = 0;

	for (size_t i = 0; i < getVertexCount() - 1; ++i)
	{
		sum += shape[i].position.x * shape[i + 1].position.y;
	}
	sum += shape[getVertexCount() - 1].position.x * shape[0].position.y;

	for (size_t i = 0; i < getVertexCount() - 1; ++i)
	{
		sum -= shape[i].position.y * shape[i + 1].position.x;
	}
	sum -= shape[getVertexCount() - 1].position.y * shape[0].position.x;

	return std::abs(sum) / 2;
}

void Shape::rotate(const float& angle)
{
	if (vertex_sSize == 0) { return; }

	float radians = angle * PI / 180;
	//sf::Vector2f Oldposition = position;

	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		sf::Vector2f& vertexPosition = shape[i].position;
		sf::Vector2f newVertexPOsition;
		newVertexPOsition.x = cos(radians) * vertexPosition.x - sin(radians) * vertexPosition.y;
		newVertexPOsition.y = sin(radians) * vertexPosition.x + cos(radians) * vertexPosition.y;

		vertexPosition = newVertexPOsition;
	}
	position = shape.getBounds().getPosition();
	//setPosition(Oldposition);
}

void Shape::reflex(const Point& center)
{
	if (vertex_sSize == 0) { return; }

	//деление отрезков в данном отношениии

	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		sf::Vector2f& positionOfVertex = shape[i].position;
		positionOfVertex.x = 2 * center.x - positionOfVertex.x;
		positionOfVertex.y = 2 * center.y - positionOfVertex.y;
	}

	position = shape.getBounds().getPosition();
}

void Shape::reflex(const Line& axis)
{
	if (vertex_sSize == 0) { return; }
	sf::Vector2f folowVector;
	folowVector.x = axis.pointB.x - axis.pointA.x;
	folowVector.y = axis.pointB.y - axis.pointA.y;
	float& a = folowVector.x;
	float& b = folowVector.y;

	float t = 1;

	Point pointOfReflex;
	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		sf::Vector2f& vertexPosition = shape[i].position;
		t = (a*(vertexPosition.y - axis.pointA.y) - b*(vertexPosition.x - axis.pointA.x)) / (b*b + a*a);

		pointOfReflex.x = vertexPosition.x + b * t;
		pointOfReflex.y = vertexPosition.y - a * t;

		vertexPosition.x = 2 * pointOfReflex.x - vertexPosition.x;
		vertexPosition.y = 2 * pointOfReflex.y - vertexPosition.y;
	}

	position = shape.getBounds().getPosition();

}

void Shape::gomotetia(const Point& point, float coef)
{

	if (vertex_sSize == 0) { return; }

	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		sf::Vector2f& vertexPosition = shape[i].position;
		vertexPosition.x = coef * (vertexPosition.x - point.x) + point.x;
		vertexPosition.y = coef * (vertexPosition.y - point.y) + point.y;
	}

	position = shape.getBounds().getPosition();
	
}

void Shape::setColor(const sf::Color& color)
{
	if (vertex_sSize == 0) { return; }
	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		shape[i].color = color;
	}
}

void Shape::setPosition(const sf::Vector2f& newPosition)
{
	if (vertex_sSize == 0) { return; }
	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		sf::Vector2f& positionOfVertex  = shape[i].position;
		positionOfVertex.x += newPosition.x - position.x;
		positionOfVertex.y += newPosition.y - position.y;
	}
	position = shape.getBounds().getPosition();
}

bool Shape::operator==(const Shape& anotherShape)
{
	if (vertex_sSize != anotherShape.vertex_sSize) { return false; }
	
	for (size_t i = 0; i < vertex_sSize; ++i)
	{
		if (shape[i].position != anotherShape[i].position) { return false; }
	}
	return true;
	
}
