#include "Game.h"

int main(int argc, char** argv)
{
	Game game; // The Game object

	bool success = game.Initialize();

	// Game Loop
	if (success) {
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
}