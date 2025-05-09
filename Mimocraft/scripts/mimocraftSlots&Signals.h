#pragma once
//MimocraftInclude
#include "MimocraftAlgo.h"
using namespace ash;

enum signals
{
	left_mous_click = 0,
	right_mous_click = 1,
	detonate_player = 2,
	rebuild_area = 3,
	rotate_area = 4,
};


static void slot_to_detonate_player(AshCore* theCore, AshEntity& oldPLayer)
{
	theCore->popEntity("player", int(oldPLayer.getFloat("world_x")) + int(oldPLayer.getFloat("world_y")) + int(oldPLayer.getFloat("world_z")));
	AshEntity& newPlayer = theCore->getEntity("player");
	newPlayer.setTexture(theCore->getResourceManager().getTexture("player.png"));
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
static void slot_to_rotate_area(AshCore* theCore, AshEntity& player)
{
	generalCleanArea(*theCore);
	slot_to_rebuild_area(theCore, player);
}