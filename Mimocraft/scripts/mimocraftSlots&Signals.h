#pragma once
#include "../AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"
using namespace ash;
enum signals
{
	left_mous_click = 0,
	right_mous_click = 1,
	detonate_player = 2,
};

static void slot_to_detonate_player(AshCore* theCore, AshEntity& oldPLayer)
{
	theCore->popEntity("player", int(oldPLayer.getFloat("world_x")) + int(oldPLayer.getFloat("world_y")) + int(oldPLayer.getFloat("world_z")));
	AshEntity& newPlayer = theCore->getEntity("player");
	newPlayer.setTexture(theCore->getResourceManager().getTexture("player.png"));
}