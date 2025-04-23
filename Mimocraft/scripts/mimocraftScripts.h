#pragma once
#include "../AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"
#include "mimocraftSlots&Signals.h"
#define playerSpeed 5
#define deltaTime theCore->getDeltaTime().asSeconds() 
#define PI 3.1415926535

using namespace ash;

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
	//AshEntity& player = theCore.getEntity("player");
	player.moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	player.moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	player.moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
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

	actual *= float((64 * 0.70));

	player.setPosition(rotateIN(actual));
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



