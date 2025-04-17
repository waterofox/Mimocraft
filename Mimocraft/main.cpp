#include "AshEngine-Alpha-2-AP25/AshEngine/AshEngine.h"
using namespace ash;
int main()
{
	AshCore game(400, 400, 120, "test");
	game.loadScene("preview.txt");
	game.startEngine();
}