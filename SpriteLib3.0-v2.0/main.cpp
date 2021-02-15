#include "Game.h"
#include "Tone Fire/Tonefire.h"


#undef main
int main()
{
	ToneFire::FMODCore fmod{};

	fmod.Update();
	//Create the Game
	Game theGame;

	//Initialize the game
	theGame.InitGame();

	//Runs the game until it returns :)
	return theGame.Run();
}