#pragma once
#include "../AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"

#define playerSpeed 200
#define deltaTime theCore->getDeltaTime().asSeconds() 

using namespace ash;

static void keyBoardChecker(AshCore& theCore, const sf::Keyboard::Key& key, const bool& move)
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
}

static void playerInput(AshCore& theCore)
{
	sf::Event& event = theCore.getActualEvent();
	sf::RenderWindow& window = theCore.getWindow();
	if (window.pollEvent(event))
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
	if (player.moveUp) { player.move(0, -1 * playerSpeed * deltaTime); }
	if (player.moveDown) { player.move(0, 1 * playerSpeed * deltaTime); }

	if (player.moveRight) { player.move(1 * playerSpeed * deltaTime, 0); }
	if (player.moveLeft) { player.move(-1 * playerSpeed * deltaTime,0); }
}



