#pragma once
#include "scripts/mimocraftScripts.h"
#include "scripts/blocks.h"
using namespace ash;

struct BlockInfo
{
	int type = Blocks::void_b;
	int x = 0;
	int y = 0;
	int z = 0;
	unsigned int hp = 0;
};

class Game : public AshCore
{
private:
	AshEntity player;
	std::string chunkDir = "data/cnunks/";
public:


	Game() : AshCore(1000, 1000, 120, "Mimocraft") { loadScene("playerArea.txt"); init(); }

	void init();

	//void areaBuilder();

	//Parsers
	int blockTypeParser(const std::string& type);
	sf::Vector2f rotate(const sf::Vector2f& cords);

	std::vector<BlockInfo> chunkParser(const std::string& chunk);
	std::vector<BlockInfo> chunkGenerator(const std::string& chunk);


};

