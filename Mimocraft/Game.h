#pragma once
#include "scripts/mimocraftScripts.h"
using namespace ash;

class Game : public AshCore
{
public:
	Game() : AshCore(1000, 1000, 120, "Mimocraft") { initGame(); }

	void initGame();
	void initPLayer();
};

