#pragma once
#include "mimocraftSlots&Signals.h"
#define playerSpeed 5
#define deltaTime theCore->getDeltaTime().asSeconds() 

using namespace ash;

static void keyBoardChecker(AshCore& theCore, const sf::Keyboard::Key& key,const  bool& move)
{
	AshEntity& player = theCore.getEntity("player");
	switch (key)
	{
	case sf::Keyboard::Key::W: {player.moveUp = move; } break;
	case sf::Keyboard::Key::D: {player.moveRight = move; } break;
	case sf::Keyboard::Key::S: {player.moveDown = move; } break;
	case sf::Keyboard::Key::A: {player.moveLeft = move; } break;
	default:
		break;
	}

	player.moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	player.moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	player.moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	
	//rotate?
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		actualSide = myMod(actualSide + 1, 4);
		theCore.emitSignal(rotate_area,player);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		actualSide = myMod(actualSide - 1, 4);
		theCore.emitSignal(rotate_area, player);
	}
}

static void playerInput(AshCore& theCore)
{
	sf::Event& event = theCore.getActualEvent();
	sf::RenderWindow& window = theCore.getWindow();
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: {keyBoardChecker(theCore,event.key.code, true); } break;
		case sf::Event::KeyReleased: {keyBoardChecker(theCore,event.key.code, false); } break;
		default:
			break;
		}

	}
}

static void playerScript(AshCore* theCore, AshEntity& player)
{

	if (player.getBool("updated")) { player.getBool("updated") = false; return; }

	float x = player.getFloat("world_x");
	float y = player.getFloat("world_y");
	
	sf::Vector2f actual(x, y);


	if (player.moveUp) { actual.y -= playerSpeed * deltaTime; }
	if (player.moveDown) { actual.y += playerSpeed * deltaTime; }

	if (player.moveLeft) { actual.x -= playerSpeed * deltaTime; }
	if (player.moveRight) { actual.x += playerSpeed * deltaTime; }

	

	if (actual.x < 0 or actual.y < 0) { return; }

	player.getFloat("world_x") = actual.x;
	player.getFloat("world_y") = actual.y;
	sf::Vector2f temp = actual;

	sf::Vector2i actualChunk;
	actualChunk.x = int(actual.x / chunkSize);
	actualChunk.y = int(actual.y / chunkSize);
	if (actualChunk.x != player.getInt("pre_chunk_x") or actualChunk.y != player.getInt("pre_chunk_y"))
	{
		player.getInt("pre_chunk_x") = actualChunk.x;
		player.getInt("pre_chunk_y") = actualChunk.y;
		theCore->emitSignal(rebuild_area, player);
	}


	actual *= float((64 * 0.70));

	player.setPosition(rotate(actual,45));
	player.move(0, -16);
	player.move(0, -(32.0*1.5) * player.getFloat("world_z"));
	player.move(0, -16*player.getFloat("world_y"));
	player.move(0, -16 * player.getFloat("world_x"));

	
	if (int(temp.x) + int(temp.y) != int(x) + int(y))
	{
		AshEntity PlayerTwo = player;
		int newLay = int(player.getFloat("world_z")) + int(temp.x) + int(temp.y);
		int oldLay = int(x) + int(y) + int(player.getFloat("world_z"));

		if (newLay > oldLay) { PlayerTwo.getBool("updated") = true; }

		theCore->pushEntity(PlayerTwo, newLay);
		player.getFloat("world_x") = x;
		player.getFloat("world_y") = y;
		theCore->emitSignal(detonate_player, player);
	}

}



