#pragma once
#include "../AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"
#include "blocks.h"
#include <set>
#include <cmath>
#define chunkSize 6
#define PI 3.1415926535
using namespace ash;

struct blockCords
{
	int x = 0;
	int y = 0;
	int z = 0;
	
	bool operator<(const blockCords& another) const
	{
		if (x == another.x)
		{
			if (y == another.y)
			{
				if (z == another.z)
				{
					return false;
				}
				else { return z < another.z; }
			}
			else { return y < another.y; }
		}
		else { return x < another.x; }
	}
};

struct BlockInfo
{
	int type = Blocks::void_b;
	blockCords cords;
	unsigned int hp = 0;
};

static std::map<std::string, std::map<blockCords, BlockInfo>> actualAreaInfo;
static std::string chunkDir = "data/chunks/";

static std::vector<std::string> blocksTextures = {
"00.png", "01.png","02.png","03.png", "04.png", "05.png",
"06.png" };


enum signals
{
	left_mous_click = 0,
	right_mous_click = 1,
	detonate_player = 2,
	rebuild_area = 3,
};

static sf::Vector2f rotateIN(const sf::Vector2f& cords)
{
	double radians = 45 * PI / 180;
	sf::Vector2f newPosition;
	newPosition.x = cos(radians) * cords.x - sin(radians) * cords.y;
	newPosition.y = sin(radians) * cords.x + cos(radians) * cords.y;
	return newPosition;
}
static sf::Vector2f rotateOUT(const sf::Vector2f& cords)
{
	double radians = -45 * PI / 180;
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

static std::string unite(const sf::Vector2i& cords)
{
	return std::to_string(cords.x) + '_' + std::to_string(cords.y);
}

static void slot_to_detonate_player(AshCore* theCore, AshEntity& oldPLayer)
{
	theCore->popEntity("player", int(oldPLayer.getFloat("world_x")) + int(oldPLayer.getFloat("world_y")) + int(oldPLayer.getFloat("world_z")));
	AshEntity& newPlayer = theCore->getEntity("player");
	newPlayer.setTexture(theCore->getResourceManager().getTexture("player.png"));
}

static void saveChunk(const std::string& name)
{
	std::ofstream chunk(chunkDir + name + ".txt", std::ios::out);
	int count = 0;
	chunk << actualAreaInfo[name].size() << '\n';
	for (auto& block : actualAreaInfo[name])
	{
		chunk << block.second.type << ' ';
		chunk << block.second.cords.x << ' ';
		chunk << block.second.cords.y << ' ';
		chunk << block.second.cords.z << "  ";
		++count;
		if (count == chunkSize) { chunk << '\n'; count = 0; }
	}
	chunk.close();
	
}
static void clearArea(AshCore& theCore, const std::string& chunk)
{
	for (auto& block : actualAreaInfo[chunk])
	{
		theCore.popEntity(std::to_string(block.second.cords.x) + ' ' + std::to_string(block.second.cords.y) + ' ' + std::to_string(block.second.cords.z),
			block.second.cords.x + block.second.cords.y + block.second.cords.z);
	}
}

static std::vector<BlockInfo> chunkParser(const std::string& chunk);

static std::vector<BlockInfo> chunkGenerator(const std::string& chunk)
{
	//сейчас просто платформу создаю

	int x = std::stoi(chunk.substr(0, chunk.find('_')));
	int y = std::stoi(chunk.substr(chunk.find('_') + 1, chunk.length() - chunk.find('_')));

	std::ofstream newChunk(chunkDir + chunk);

	newChunk << 36 << '\n';

	for (int i = 0 + y * chunkSize; i < 6 + y * chunkSize; ++i)
	{
		for (int j = 0 + x * chunkSize; j < 6 + x * chunkSize; ++j)
		{
			newChunk << "0 " << j << ' ' << i << ' ' << '0' << "  ";
		}
		newChunk << '\n';
	}
	newChunk.close();
	return chunkParser(chunk);
}


static std::vector<BlockInfo> chunkParser(const std::string& chunk)
{
	std::vector<BlockInfo> resualt;
	BlockInfo newBLock;
	std::ifstream chunk_f(chunkDir + chunk);

	if (!chunk_f.is_open())
	{
		return chunkGenerator(chunk);
	}
	int blocksCount;
	chunk_f >> blocksCount;
	for (int i = 0; i < blocksCount; ++i)
	{
		chunk_f >> newBLock.type;
		chunk_f >> newBLock.cords.x >> newBLock.cords.y >> newBLock.cords.z;

		resualt.push_back(newBLock);
	}
	chunk_f.close();
	return resualt;
}


static void loadChunk(AshCore& theCore ,const std::string& name)
{
	std::vector<BlockInfo> chunkData = chunkParser(name + ".txt");


	for (int i = 0; i < chunkData.size(); ++i)
	{
		BlockInfo& block = chunkData[i];
		AshEntity blockEntity;

		sf::Vector2f blockPosition(block.cords.x, block.cords.y);
		blockPosition *= float((64 * 0.70));
		blockEntity.setPosition(rotateIN(blockPosition));
		blockEntity.move(0, -(32 * block.cords.z + 16 * block.cords.x + 16 * block.cords.y));

		blockEntity.setName(std::to_string(block.cords.x) + ' ' + std::to_string(block.cords.y) + ' ' + std::to_string(block.cords.z));

		blockEntity.setTexturePath(blocksTextures[block.type]);
		blockEntity.setTextureRect(sf::IntRect(0, 0, 64, 64));
		blockEntity.setScale(1, 1);
		blockEntity.setVisible(true);
		blockEntity.setColliding(false);

		theCore.pushEntity(blockEntity, block.cords.x + block.cords.y + block.cords.z);

		actualAreaInfo[name][block.cords] = block;

	}
}

static void slot_to_rebuild_area(AshCore* theCore, AshEntity& player)
{
	sf::Vector2i actualChunk;
	actualChunk.x = player.getInt("pre_chunk_x");
	actualChunk.y = player.getInt("pre_chunk_y");

	std::set<std::string> newChunks;
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x,actualChunk.y))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x+1, actualChunk.y))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x+1, actualChunk.y+1))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x, actualChunk.y+1))));

	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x -1, actualChunk.y))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x - 1, actualChunk.y - 1))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x, actualChunk.y - 1))));

	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x - 1, actualChunk.y + 1))));
	newChunks.insert(unite(absVector(sf::Vector2i(actualChunk.x + 1, actualChunk.y -1 ))));

	//clear Chunks
	if (!actualAreaInfo.empty())
	{
		std::set<std::string> chunksToDelete;
		for (auto& chunk : actualAreaInfo)
		{
			if (newChunks.count(chunk.first) == 0) { chunksToDelete.insert(chunk.first); }
		}
		if (!chunksToDelete.empty())
		{
			for (auto& chunk : chunksToDelete)
			{
				saveChunk(chunk);
				clearArea(*theCore, chunk);

				actualAreaInfo.erase(chunk);
			}
		}
	}
	for (auto& chunk : newChunks)
	{
		auto iterOnChunk = actualAreaInfo.find(chunk);
		if (iterOnChunk == actualAreaInfo.end()){ 
			loadChunk(*theCore,chunk); }
	}
}