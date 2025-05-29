

#include "Game.h"


void Game::initGame()
{
	loadScene("playerArea.txt"); 
	initPLayer();

	this->addSlot("playerArea", signals::rebuild_area, slot_to_rebuild_area);
	this->addSlot("playerArea", signals::rotate_area, slot_to_rotate_area);
}

void Game::initPLayer()
{
	AshEntity player;

	//init properties
	player.addProperty(ash::p_float, "world_x", 0);
	player.addProperty(ash::p_float, "world_y", 0);
	player.addProperty(ash::p_float, "world_z", 4);
	player.addProperty(ash::p_bool, "updated", false);
	player.addProperty(ash::p_int, "pre_chunk_x", -1);
	player.addProperty(ash::p_int, "pre_chunk_y", -1);
	player.addProperty(ash::p_int, "lay", 4);
	player.addProperty(ash::p_float, "jump", 0);
	player.addProperty(ash::p_bool, "fall", false);

	//init texture
	player.setTexturePath("player.png");
	player.setTextureRect(sf::IntRect(0, 0, 64, 64));
	player.setScale(1.5,1.5);
	player.setOrigin(8, 24);
	//add
	player.setName("player");
	this->pushEntity(player,int(player.getFloat("world_x") + int(player.getFloat("world_y") + int(player.getFloat("world_z")))));

	//scripting
	this->addScript("playerArea", "player", playerScript);
	this->setEventHandlingFunction(playerInput);
	this->addSlot("playerArea", signals::detonate_player, slot_to_detonate_player);
	this->addSlot("playerArea", signals::place_block, slot_to_place_block);
	this->addSlot("playerArea", signals::win, slot_to_win);
	this->addSlot("playerArea", signals::loose, slot_to_loose);
	//animation;
	AshAnimator& animator = this->getAnimator();
	animator.addAnimation("player.png", 4, 10, true);

}
