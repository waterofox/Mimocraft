#define chunkSize 6
#define PI 3.1415926535

#include "Game.h"

std::vector<std::string> blocksTextures = { 
"00.png", "01.png","02.png","03.png", "04.png", "05.png",
"06.png" };

void Game::init()
{

	player.addProperty(ash::p_float, "world_x", 0);
	player.addProperty(ash::p_float, "world_y", 0);
	player.addProperty(ash::p_float, "world_z", 0);

	player.setTexturePath("sq.png");
	AshResourceManager::textureSettings settings;
	settings.sRgb = false; settings.repeated = true, settings.smooth = false;
	this->getResourceManager().addTexture("sq.png", settings);
	player.setTextureRect(sf::IntRect(0, 0, 64, 80));
	player.setScale(1, 1);

	player.setName("player");
	this->pushEntity(player,0);
	this->addScript("playerArea", "player", playerScript);

	this->setEventHandlingFunction(playerInput);

	areaBuilder();
}

void Game::areaBuilder()
{
	sf::Vector2f realPLayerPos(player.getFloat("world_x"), player.getFloat("world_y"));
	
	std::string chunkName = std::to_string(int(realPLayerPos.x / chunkSize)) + "_" + std::to_string(int(realPLayerPos.y / chunkSize));
	std::vector<BlockInfo> chunkData = chunkParser(chunkName + ".txt");

	for (int i = 0; i < chunkData.size(); ++i)
	{
		BlockInfo& block = chunkData[i];
		AshEntity blockEntity;

		sf::Vector2f blockPosition(block.x, block.y);
		blockPosition *= float((64 * 0.70));
		blockEntity.setPosition(rotateIN(blockPosition));
		blockEntity.move(0,-(32*block.z + 16 * myMod(block.x,chunkSize) + 16*myMod(block.y,chunkSize)));

		blockEntity.setName(std::to_string(block.x) + ' ' + std::to_string(block.y) + ' ' + std::to_string(block.z));

		blockEntity.setTexturePath(blocksTextures[block.type]);
		blockEntity.setTextureRect(sf::IntRect(0, 0, 64, 64));
		blockEntity.setScale(1, 1);
		blockEntity.setVisible(true);
		blockEntity.setColliding(false);

		this->pushEntity(blockEntity, block.z);
		
	}
	std::cout << "fin";
}

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

sf::Vector2f Game::rotateIN(const sf::Vector2f& cords)
{
	double radians = 45 * PI / 180;
	sf::Vector2f newPosition;
	newPosition.x = cos(radians) * cords.x - sin(radians) * cords.y;
	newPosition.y = sin(radians) * cords.x + cos(radians) * cords.y;
	return newPosition;
}

sf::Vector2f Game::rotateOUT(const sf::Vector2f& cords)
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

	if (!chunk_f.is_open()) 
	{
		return chunkGenerator(chunk); 
	}
	
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

			if (newBLock.type == Blocks::void_b) { continue; }
			resualt.push_back(newBLock);
		}
	}
	return resualt;
}

std::vector<BlockInfo> Game::chunkGenerator(const std::string& chunk)
{
	return std::vector<BlockInfo>();
}
