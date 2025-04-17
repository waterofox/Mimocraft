#define chunkSize 6
#define PI 3.1415926535

#include "Game.h"

int Game::blockTypeParser(const std::string& type)
{
	if (type == "-1") { return Blocks::void_b; }
	if (type == "00") { return Blocks::grass; }
	if (type == "01") { return Blocks::solid_stone; }
	if (type == "02") { return Blocks::woods; }
	if (type == "03") { return Blocks::red_fur; }
	if (type == "04") { return Blocks::cobble_stone; }
	if (type == "05") { return Blocks::water; }
	if (type == "06") { return Blocks::glass; }
}

sf::Vector2f Game::rotate(const sf::Vector2f& cords)
{
	double radians = -45 * PI / 180;
	sf::Vector2f newPosition;
	newPosition.x = cos(radians) * cords.x - sin(radians) * cords.y;
	newPosition.y = sin(radians) * cords.x + cos(radians) * cords.y;
	return newPosition;
}

std::vector<BlockInfo> Game::chunkParser(const std::string& chunk)
{
	std::vector<BlockInfo> resualt;
	BlockInfo newBLock;
	std::ifstream chunk_f(chunkDir + chunk);

	if (!chunk_f.is_open()) { return chunkGenerator(chunk); }
	
	int countOfLays;
	chunk_f >> countOfLays;
	std::string type;
	for (int l = 0; l < countOfLays; ++l)
	{
		for (int i = 0; i < chunkSize * chunkSize; ++i)
		{
			chunk_f >> type;
			newBLock.type = blockTypeParser(type);
			chunk_f >> newBLock.x >> newBLock.y >> newBLock.z;

			resualt.push_back(newBLock);
		}
	}
	return resualt;
}
