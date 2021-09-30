#include "Game.h"
// Used to print Tests
#include <iostream>

/*
* 
* Dungeon Delver has a character exploring a Dungeon in the Dark
* They can use the right mouse button to Create Light, and the Left to Shoot Fireballs
* Character moves with WASD keys
* Monsters walk around in the dark and will approach the character when spotted
* Monsters can see better in the dark ( larger sight radius )
* 
* 
* TODO
* Add fog of war
* Add player Light - remove fog
* 
* Nice to have...
* Make Fireball / skeleton move in a straight line
* prevent skeletons from spawning in player line of sight
* pickups for score?
* Sounds continue to play after the entity is destroyed (until chunk ends)
* fireball explosion sound can start while fireball creation sound has not finished
* 
* Animation Tutorial
* http://www.sdltutorials.com/sdl-animation
* Text Tutorial
* https://www.lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
* 
* Sounds
* https://opengameart.org/content/a-la-poursuite-dune-ombre-du-pass%C3%A9 - background
* https://freesound.org/people/LiamG_SFX/sounds/334237/ - Fireball
* https://freesound.org/people/HighPixel/sounds/431174/ - fireball explosion
* https://freesound.org/people/landlucky/sounds/277403/ - game over
* https://freesound.org/people/cribbler/sounds/381859/ - skeleton destroy
* https://freesound.org/people/swuing/sounds/38873/ - steps?
* 
* GUI graphics
* https://opengameart.org/content/rpg-gui-construction-kit-v10 
* 
* https://www.bing.com/images/search?view=detailV2&ccid=4eKqKkdx&id=9450EA8D7854EBF1A85E0373981B15C9BF345939&thid=OIP.4eKqKkdxOVGVGL1iIAgyfQAAAA&mediaurl=http%3a%2f%2fi.imgur.com%2fk296H.png&exph=256&expw=256&q=Pixel+Explosion+Sprites+Sheets&simid=608016208222751175&ck=54B8BD8C610F5F9C72CF45C84CF7EA51&selectedIndex=12&qft=+filterui%3alicense-L2_L3_L5_L6&FORM=IRPRST&ajaxhist=0
* https://www.bing.com/images/search?view=detailV2&ccid=puWtKzqb&id=4984604430936E7A3A1FAA8B4774483692118667&thid=OIP.puWtKzqb_S1Tv9zjALygdQHaDS&mediaurl=https%3a%2f%2fi.stack.imgur.com%2fxPf2J.png&exph=256&expw=576&q=Pixelated+Fireball+Sprite+Sheet&simid=608026168178313013&ck=4649B6F5F2C22C3EF3130E6BB13FE960&selectedIndex=10&qft=+filterui%3alicense-L2_L3_L5_L6&FORM=IRPRST&ajaxhist=0
* 
* Fonts
* https://fontsnetwork.com/tiki-magic-font/
* 
*/

Game::Game() {
	mWindow = nullptr;
	mIsRunning = true;
	mRenderer = nullptr;
	mFogSurface = nullptr;
	mScreenSurface = nullptr; 
	audio = Audio();

	mScreenWidth = 1920;
	mScreenHeight = 1080;
	mTicksCount = 0;
	makeLight = false;

	isPaused = false;
}

SDL_Texture* Game::LoadImage(const char* fileName)
{
	SDL_Surface* imageSurface = SDL_LoadBMP(fileName);
	// if it doesn't exist...
	if (imageSurface == nullptr) {
		SDL_Log("Unable to load IMAGE");
		return false;
	}

	// Color key - removes white background from my modified sprites
	Uint32 colorkey = SDL_MapRGB(imageSurface->format, 255, 255, 255);
	SDL_SetColorKey(imageSurface, SDL_TRUE, colorkey);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, imageSurface);
	SDL_FreeSurface(imageSurface); // destroy unneeded surface
	return texture;
}

void Game::LoadTextures() {
	// Characters
	//Player
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerDown1", LoadImage("Assets\\Player\\down1.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerDown2", LoadImage("Assets\\Player\\down2.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerDown3", LoadImage("Assets\\Player\\down3.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerUp1", LoadImage("Assets\\Player\\up1.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerUp2", LoadImage("Assets\\Player\\up2.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerUp3", LoadImage("Assets\\Player\\up3.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerLeft1", LoadImage("Assets\\Player\\left1.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerLeft2", LoadImage("Assets\\Player\\left2.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerLeft3", LoadImage("Assets\\Player\\left3.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerRight1", LoadImage("Assets\\Player\\right1.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerRight2", LoadImage("Assets\\Player\\right2.bmp")));
	playerTextureMap.insert(pair<string, SDL_Texture*>("playerRight3", LoadImage("Assets\\Player\\right3.bmp")));
	//Skeleton
	textureMap.insert(pair<string, SDL_Texture*>("up1", LoadImage("Assets\\Skeleton\\up1.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("up2", LoadImage("Assets\\Skeleton\\up2.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("up3", LoadImage("Assets\\Skeleton\\up3.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("left1", LoadImage("Assets\\Skeleton\\left1.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("left2", LoadImage("Assets\\Skeleton\\left2.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("left3", LoadImage("Assets\\Skeleton\\left3.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("right1", LoadImage("Assets\\Skeleton\\right1.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("right2", LoadImage("Assets\\Skeleton\\right2.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("right3", LoadImage("Assets\\Skeleton\\right3.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("down1", LoadImage("Assets\\Skeleton\\down1.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("down2", LoadImage("Assets\\Skeleton\\down2.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("down3", LoadImage("Assets\\Skeleton\\down3.bmp")));
	//Fireball
	textureMap.insert(pair<string, SDL_Texture*>("fireball", LoadImage("Assets\\fireball.bmp")));

	// GUI
	textureMap.insert(pair<string, SDL_Texture*>("background", LoadImage("Assets\\Background.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("menu", LoadImage("Assets\\menu.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("buttonUp", LoadImage("Assets\\buttonUP.bmp")));
	textureMap.insert(pair<string, SDL_Texture*>("buttonDown", LoadImage("Assets\\buttonDOWN.bmp")));
}

void Game::CreateCharacters() {
	// Create Character Objects
	mPlayer = Player(mScreenWidth / 2, mScreenHeight - 50, playerTextureMap);
	mPlayer.SetCurrentAnims("start");

	// Place into Array
	enemyCharacters[0] = Skeleton(50,500, textureMap);
	enemyCharacters[1] = Skeleton(900, 450, textureMap);
	enemyCharacters[2] = Skeleton(1300, 100, textureMap);
	enemyCharacters[3] = Skeleton(500, 625, textureMap);
	enemyCharacters[4] = Skeleton(1500, 700, textureMap);
	enemyCharacters[5] = Skeleton(200, 800, textureMap);

	fireball = Fireball();
}

bool Game::Initialize() {
	
	bool success = true;

	//Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	// Check Success
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		success = false;
	}

	// Get actual Screen Size
	if (SDL_GetDesktopDisplayMode(0, &mDisplayMode) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return 1;
	}
	else {
		mScreenWidth = mDisplayMode.w;
		mScreenHeight = mDisplayMode.h;

		// Practice with 1080p
		mScreenWidth = 1920;
		mScreenHeight = 1080;
	}
	
	// Initialize SDL window 
	mWindow = SDL_CreateWindow(
		"Project Three",	// Title
		50,					// Top left x-coordinate
		50,					// Top left y-coordinate
		mScreenWidth,		// width
		mScreenHeight,		// height
		0					// Flags (0 means there are none)
	);

	// Check Success
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		success = false;
	}

	// This will change the screen background color
	mScreenSurface = SDL_GetWindowSurface(mWindow);
	SDL_FillRect(mScreenSurface,
		NULL,
		SDL_MapRGB(
			mScreenSurface-> format,
			255,
			255,
			255
		)
	);

	mScreenRect = { 0, 0, mScreenWidth, mScreenHeight };

	// Create Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	else {
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0xFF);
	}

	TTF_Init();

	SDL_UpdateWindowSurface(mWindow);
	// Load Game Textures
	LoadTextures();
	// Create Characters
	CreateCharacters();

	audio.InitializeAudio();
	scoreInt = 0;

	// if all went well... initialization is a success
	return success;
}

void Game::RunLoop() {

	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {

	SDL_Event event;
	mPlayer.SetPlayerDirection(0, 0);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				// Keep Fireball in the gameScreen
				if (x < mScreenWidth - 300) {
					Vector2 target = { (float)x, (float)y };
					// SHOOT A FIREBALL at SDL_GetMouseState location
					fireball = Fireball(mPlayer.GetPlayerPosition().x, mPlayer.GetPlayerPosition().y, textureMap["fireball"], target);
				}
				// Press Quit - if is checking the cooridinated of the Quit Button
				if ((x > 1720 && x < 1820) && (y > 950 && y < 1000)) {
					mIsRunning = false;
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				// TEST
				std::cout << "RIGHT MOUSE: LIGHT! \n";

				// CAST LIGHT
				makeLight = true;
			}
		}
	}
	// Handle Player movement Input
	mPlayer.GetInput(event, mIsRunning, mScreenHeight, mScreenWidth);
}

void Game::UpdateGame() {

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	// Move Player
	mPlayer.UpdatePlayer(deltaTime);
	// Move Enemies
	for (int iterator = 0; iterator < 6; iterator++) {
		// Skeleton will only move if they can see the playeraa
		if (enemyCharacters[iterator].CanSeePlayer(mPlayer.GetCollider())) {
			enemyCharacters[iterator].UpdateSkeleton(deltaTime, mPlayer.GetPlayerPosition());
		}
		// check for any player collisions with enemies
		if (mPlayer.CheckCollision(enemyCharacters[iterator].GetCollider())) {
			mPlayer.DestroyPlayer();
			mIsRunning = false;
		}

		fireball.UpdateFireball(deltaTime);
		for (int iterator = 0; iterator < 6; iterator++) {
			if (fireball.CheckCollision(enemyCharacters[iterator].GetCollider())) {
				scoreInt += 100;
				fireball.DestroyFireball();
				int x = rand() % 1500;
				int y = rand() % 1000;

				enemyCharacters[iterator].DestroySkeleton();
				enemyCharacters[iterator] = Skeleton(x, y, textureMap);
				enemyCharacters[iterator].Respawn(mPlayer.GetPlayerPosition().x, mPlayer.GetPlayerPosition().y);
			}
		}
	}
}

void Game::GenerateOutput() {

	SDL_RenderClear(mRenderer);

	// Game ViewPort
	SDL_Rect gameViewPort;
	gameViewPort.x = 0;
	gameViewPort.y = 0;
	gameViewPort.w = mScreenWidth - 300;
	gameViewPort.h = mScreenHeight;
	SDL_RenderSetViewport(mRenderer, &gameViewPort);

	// Game Background
	SDL_RenderCopy(mRenderer, textureMap["background"], NULL, NULL);

	// Try and Put both Player AND Skeleton in same array? - polymorphism
	for (int iterator = 0; iterator < 6; iterator++) {
		if (enemyCharacters[iterator].isToClose == false) {
			enemyCharacters[iterator].RenderCharacter(mRenderer);
		}
	}

	// Render Player
	mPlayer.RenderCharacter(mRenderer);
	// Render Fireball
	fireball.RenderCharacter(mRenderer);

	// Render Fog Of War
	SDL_SetRenderDrawColor(mRenderer, 22, 22, 22, 255);
	SDL_Surface* mFoggySurface = SDL_CreateRGBSurface(SDL_SWSURFACE, mScreenWidth - 300, mScreenHeight, 32, 255, 255, 255, 0);
	SDL_Rect screenRect = { 0, 0, gameViewPort.w, gameViewPort.h };
	//SDL_RenderFillRect(mRenderer, &screenRect);

	SDL_Rect Dest = { mScreenWidth, mScreenHeight, 0, 0 };
	SDL_BlitSurface(mFogSurface, NULL, mScreenSurface, NULL);
	
	//
	// Everything after this point renders in the Menu ViewPort
	//

	// The Menu ViewPort
	SDL_Rect menuViewPort;
	menuViewPort.x = mScreenWidth - 300;
	menuViewPort.y = 0;
	menuViewPort.w = 300;
	menuViewPort.h = mScreenHeight;
	SDL_RenderSetViewport(mRenderer, &menuViewPort);

	//Render Background Texture
	SDL_RenderCopy(mRenderer, textureMap["menu"], NULL, NULL);

	// Add a title
	SDL_Rect titleRect = { 50, 50, 200, 50 };
	TTF_Font* font = TTF_OpenFont("Assets\\Fonts\\TikiMagic.ttf", 32);
	SDL_Color white = { 250,250,250 };
	SDL_Surface* titleMessage = TTF_RenderText_Solid(font, "Dungeon Delver", white);
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(mRenderer, titleMessage);
	SDL_RenderCopy(mRenderer, titleTexture, NULL, &titleRect);
	SDL_FreeSurface(titleMessage);
	SDL_DestroyTexture(titleTexture);

	// Add Score Label
	SDL_Rect scoreRect = {80, 300, 80, 50 };
	font = TTF_OpenFont("Assets\\Fonts\\TikiMagic.ttf", 24);
	SDL_Surface* scoreMessage = TTF_RenderText_Solid(font, "Score", white);
	SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(mRenderer, scoreMessage);
	SDL_RenderCopy(mRenderer, scoreTexture, NULL, &scoreRect);
	SDL_FreeSurface(scoreMessage);
	SDL_DestroyTexture(scoreTexture);

	// Add Score
	SDL_Rect scorePointsRect = { 180, 300, 50, 50 };
	font = TTF_OpenFont("Assets\\Fonts\\TikiMagic.ttf", 24);
	points = to_string(scoreInt);
	score = points.c_str();
	SDL_Surface* scorePointsMessage = TTF_RenderText_Solid(font, score, white);
	SDL_Texture* scorePointsTexture = SDL_CreateTextureFromSurface(mRenderer, scorePointsMessage);
	SDL_RenderCopy(mRenderer, scorePointsTexture, NULL, &scorePointsRect);
	SDL_FreeSurface(scorePointsMessage);
	SDL_DestroyTexture(scorePointsTexture);

	// Add Quit
	SDL_Rect quitRect = { 100, 950, 100, 50 };
	font = TTF_OpenFont("Assets\\Fonts\\TikiMagic.ttf", 32);
	SDL_Surface* quitMessage = TTF_RenderText_Solid(font, "Quit", white);
	SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(mRenderer, quitMessage);
	SDL_RenderCopy(mRenderer, quitTexture, NULL, &quitRect);
	SDL_FreeSurface(quitMessage);
	SDL_DestroyTexture(quitTexture);

	// Display
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	// Destroy Audio
	audio.DestroyAudio();
	// Destroy Renderer
	SDL_DestroyRenderer(mRenderer); 
	mRenderer = nullptr;
	// Destroy Screen
	SDL_FreeSurface(mScreenSurface);
	SDL_FreeSurface(mFogSurface);
	mFogSurface = nullptr;
	mScreenSurface = nullptr;
	// Destroy Window
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	SDL_Quit();
}