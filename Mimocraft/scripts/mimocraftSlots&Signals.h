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
	place_block = 5,
	win = 6,
	loose = 7,
	detonate_area = 8,
};
static void slot_to_win(AshCore* theCore, AshEntity& oldPLayer)
{
	if (!winCreated)
	{
		std::cout << "You win!!! Press enter";
		winCreated = true;
	}
	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) { theCore->getWindow().close(); }
		break;
	}
}
static void slot_to_denotate_area(AshCore* theCore, AshEntity& oldPLayer)
{
	generalCleanArea(*theCore);
}
static void slot_to_loose(AshCore* theCore, AshEntity& oldPLayer)
{
	if (!looseCreated)
	{
		std::cout << "You Loose!!! Press enter";
		looseCreated = true;
	}
	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) { theCore->getWindow().close(); }
		break;
	}
}

static void slot_to_detonate_player(AshCore* theCore, AshEntity& oldPLayer)
{
	theCore->popEntity("player", oldPLayer.getInt("lay"));

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
	oldSide = actualSide;
	
	AshEntity PlayerTwo = player;
	sf::Vector2f newPLayerPos(player.getFloat("world_x"), player.getFloat("world_y"));
	newPLayerPos = rotateCordsBySide(newPLayerPos, true);
	deployPlayer(PlayerTwo, newPLayerPos);
	int newLay = int(newPLayerPos.x) + int(newPLayerPos.y) + int(player.getFloat("world_z"));
	if (newLay != PlayerTwo.getInt("lay"))
	{
		PlayerTwo.getInt("lay") = newLay;
		theCore->pushEntity(PlayerTwo, newLay);
		theCore->emitSignal(detonate_player, player);
		return;

	}
	player = PlayerTwo;
	player.setTexture(theCore->getResourceManager().getTexture("player.png"));
}
static void slot_to_remove_block(AshCore* theCore, AshEntity& player)
{
	if (lastLightBlock != nullptr)
	{
		std::string name = lastLightBlock->getName();
		blockCords cords;
		cords.x = (*lastLightBlock).getInt("world_x");
		cords.y = (*lastLightBlock).getInt("world_y");
		cords.z = (*lastLightBlock).getInt("world_z");

		sf::Vector2f rot(cords.x, cords.y); rot = rotateCordsBySide(rot, true);
		theCore->popEntity(name, int(rot.x) + int(rot.y) + cords.z);
		if (actualAreaInfo[its(cords.x / chunkSize) + '_' + its(cords.y / chunkSize)][cords].type == Blocks::snow) { --snowCount; }
		actualAreaInfo[its(cords.x / chunkSize) + '_' + its(cords.y / chunkSize)].erase(cords);

		lastLightBlock = nullptr;
	}
}
static void slot_to_place_block(AshCore* theCore, AshEntity& player)
{
	BlockInfo block;
	block.type = tookedBlock;
	if (tookedBlock == Blocks::snow) { ++snowCount; }
	if (lastLightBlock != nullptr)
	{
		blockCords& cords = block.cords;
		cords.x = (*lastLightBlock).getInt("world_x");
		cords.y = (*lastLightBlock).getInt("world_y");
		cords.z = (*lastLightBlock).getInt("world_z");
		switch (sideOfLastLigthBlock)
		{
		case blockSides::top:   {cords.z += 1;} break;
		case blockSides::left:  {cords.y += 1;} break;
		case blockSides::rigth: {cords.x += 1;} break;
		default:
			break;
		}
		if (actualSide != sides::South and sideOfLastLigthBlock != blockSides::top)
		{
			switch (actualSide)
			{
			case sides::East: 
			{
				cords.y -= 1;
				if (sideOfLastLigthBlock == blockSides::left) { cords.x += 1; }
				else{cords.x -= 1;}
			}break;
			case sides::North: 
			{
				if (sideOfLastLigthBlock == blockSides::left) { cords.y -= 2; }
				else { cords.x -= 2; }
			} break;
			case sides::West: 
			{
				cords.x -= 1;
				if (sideOfLastLigthBlock == blockSides::left) { cords.y -= 1; }
				else { cords.y += 1; }
			} break;
			default:
				break;
			}
		}
		deplyBlock(*theCore, block, its(cords.x / chunkSize) + '_' + its(cords.y / chunkSize));
	}
}