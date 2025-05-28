#pragma once
#include "mimocraftSlots&Signals.h"
#define playerSpeed 3
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
		theCore.emitSignal(rotate_area, player);
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
	AshEntity& player = theCore.getEntity("player");
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed: {
			if (event.key.code == sf::Mouse::Button::Right)
			{
				slot_to_place_block(&theCore, player);
			}

		} break;
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: {keyBoardChecker(theCore,event.key.code, true); } break;
		case sf::Event::KeyReleased: {keyBoardChecker(theCore,event.key.code, false); } break;
		default:
			break;
		}

		sf::Vector2f cursor(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		detectBlock(&theCore, player,cursor);
	}
}


static void playerScript(AshCore* theCore, AshEntity& player)
{
	if (player.getBool("updated")) { player.getBool("updated") = false; return; }

	sf::Vector2f realPlayerPositionInWorld(player.getFloat("world_x"), player.getFloat("world_y"));
	sf::Vector2f oldPosition(realPlayerPositionInWorld);
	oldPosition = rotateCordsBySide(oldPosition, true);

	bool move = false;

	if (player.moveDown) {
		realPlayerPositionInWorld += sf::Vector2f(0, 1 * playerSpeed * deltaTime); move = true;
	}
	if (player.moveLeft) { realPlayerPositionInWorld += sf::Vector2f(-1 * playerSpeed * deltaTime, 0); move = true; }
	if (player.moveUp) { realPlayerPositionInWorld += sf::Vector2f(0, -1 * playerSpeed * deltaTime);  move = true; }
	if (player.moveRight) { realPlayerPositionInWorld += sf::Vector2f(1 * playerSpeed * deltaTime, 0); move = true; }

	AshAnimator& animator = theCore->getAnimator();
	ash::AshAnimator::animation& anime = animator.getAnimation(player.getTexturePath());
	if (!move and anime.enable) { anime.currentFrame = 0; anime.enable = false; }
	if (move and !anime.enable) { anime.enable = true; }


	if (realPlayerPositionInWorld.x < 0 or realPlayerPositionInWorld.y < 0) { return; }

	player.getFloat("world_x") = realPlayerPositionInWorld.x;
	player.getFloat("world_y") = realPlayerPositionInWorld.y;


	sf::Vector2i actualChunk;
	actualChunk.x = int(realPlayerPositionInWorld.x / chunkSize);
	actualChunk.y = int(realPlayerPositionInWorld.y / chunkSize);
	if (actualChunk.x != player.getInt("pre_chunk_x") or actualChunk.y != player.getInt("pre_chunk_y"))
	{
		player.getInt("pre_chunk_x") = actualChunk.x;
		player.getInt("pre_chunk_y") = actualChunk.y;
		theCore->emitSignal(rebuild_area, player);
	}


	sf::Vector2f actualPlayerCords(realPlayerPositionInWorld);
	actualPlayerCords = rotateCordsBySide(actualPlayerCords, true);

	deployPlayer(player, actualPlayerCords);

	int newLay = int(actualPlayerCords.x) + int(actualPlayerCords.y) + int(player.getFloat("world_z"));
	if (newLay != player.getInt("lay"))
	{
		if (newLay > player.getInt("lay")) { player.getBool("updated") = true; }

		AshEntity PlayerTwo = player;
		PlayerTwo.getInt("lay") = newLay;
		theCore->pushEntity(PlayerTwo, newLay);
		theCore->emitSignal(detonate_player, player);
	}
}



