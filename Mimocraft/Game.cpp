

#include "Game.h"


void Game::initGame()
{
	loadScene("playerArea.txt"); 
	initPLayer();

	this->addSlot("playerArea", signals::rebuild_area, slot_to_rebuild_area);
}

void Game::initPLayer()
{
	AshEntity player;

	//init properties
	player.addProperty(ash::p_float, "world_x", 0);
	player.addProperty(ash::p_float, "world_y", 0);
	player.addProperty(ash::p_float, "world_z", 1);
	player.addProperty(ash::p_bool, "updated", false);
	player.addProperty(ash::p_int, "pre_chunk_x", -1);
	player.addProperty(ash::p_int, "pre_chunk_y", -1);

	//init texture
	player.setTexturePath("player.png");
	player.setTextureRect(sf::IntRect(0, 0, 64, 64));
	player.setScale(1.5,1.5);

	//add
	player.setName("player");
	this->pushEntity(player,int(player.getFloat("world_x") + int(player.getFloat("world_y") + int(player.getFloat("world_z")))));

	//scripting
	this->addScript("playerArea", "player", playerScript);
	this->setEventHandlingFunction(playerInput);
	this->addSlot("playerArea", signals::detonate_player, slot_to_detonate_player);
	

}
