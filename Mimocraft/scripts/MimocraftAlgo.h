#pragma once
//AshEngine
#include "../AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"
//Mimocraft
#include "blocks.h"
#include "Algoritm.h"
#define chunkSize 6

//std
#include <set>

using namespace ash;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
enum sides
{
	South = 0,
	East = 1,
	North = 2,
	West = 3,
};

static std::map<std::string, std::map<blockCords, BlockInfo>> actualAreaInfo;
static std::string chunkDir = "data/chunks/";

static std::vector<std::string> blocksTextures = {
"00.png", "01.png","02.png","03.png", "04.png", "05.png",
"06.png" };

static int actualSide = sides::South;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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

//просто объ€вил функцию выше, что бы еЄ можно было заюзать
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


static void loadChunk(AshCore& theCore, const std::string& name)
{
	std::vector<BlockInfo> chunkData = chunkParser(name + ".txt");


	for (int i = 0; i < chunkData.size(); ++i)
	{
		BlockInfo& block = chunkData[i];
		AshEntity blockEntity;


		sf::Vector2f blockPosition(block.cords.x, block.cords.y);
		blockPosition = rotate(blockPosition, actualSide*90); //area rotation S E N W 

		blockPosition *= float((64 * 0.70));
		blockEntity.setPosition(rotate(blockPosition,45));
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

static void generalCleanArea(AshCore& theCore)
{
	for (auto& chunk : actualAreaInfo)
	{
		for (auto& block : chunk.second)
		{
			theCore.popEntity(std::to_string(block.second.cords.x) + ' ' + std::to_string(block.second.cords.y) + ' ' + std::to_string(block.second.cords.z),
				block.second.cords.x + block.second.cords.y + block.second.cords.z);
		}
	}
	actualAreaInfo.clear();
}