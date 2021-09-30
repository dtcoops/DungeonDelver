#pragma once
#include "SDL/SDL.h"
#include "Player.h"
#include "Skeleton.h"
#include "Fireball.h"

#include <SDL/SDL_ttf.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "Audio.h"

using namespace std;

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	// Helper functions for the Game Loop
	void ProcessInput();
	void UpdateGame();

	void GenerateOutput(); 
	void CreateCharacters();

	// member variables
	SDL_Window* mWindow;
	SDL_Surface* mScreenSurface;
	SDL_Surface* mFogSurface;
	SDL_Rect mScreenRect;
	SDL_DisplayMode mDisplayMode;
	SDL_Renderer* mRenderer;
	Audio audio;
	Player mPlayer;
	Fireball fireball;
	string points;
	const char* score;
	int scoreInt;
	// Images and Textures
	SDL_Texture* LoadImage(const char* fileName);
	void LoadTextures();
	map <string, SDL_Texture*> playerTextureMap;
	map <string, SDL_Texture*> textureMap;

	Skeleton enemyCharacters[6];
	bool mIsRunning;
	bool makeLight;
	int mScreenWidth, mScreenHeight;
	int mTicksCount;

	bool isPaused;
};
