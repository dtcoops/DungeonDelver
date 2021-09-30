#pragma once
#include "Character.h"
#include "Vector2.h"

#include <string>
#include <map>
#include <iostream>
#include "Audio.h"
using namespace std;

class Player : public Character
{
public:
	// Constructors
	Player();
	Player(float x, float y, map<string, SDL_Texture*> playerTextureMap);
	void SetPlayerPosition(float x, float y);
	Vector2 GetPlayerPosition();
	void SetPlayerDirection(float x, float y);
	Vector2 GetPlayerDirection();
	Circle& GetCollider();

	void GetInput(SDL_Event event, bool mIsRunning, int mScreenHeight, int mScreenWidth);
	bool CheckCollision(Circle& enemyCollider);
	void UpdatePlayer(float deltaTime);
	void SetCurrentAnims(string direction);
	void RenderCharacter(SDL_Renderer* mRenderer);
	void DestroyPlayer();
private:
	Vector2 mPlayerPosition;
	Vector2 mPlayerDirection;
	SDL_Texture* mTexture;
	map <string, SDL_Texture*> textureMap;
	SDL_Texture* currentAnims[3];
	Audio audio;
	int playerHeight;
	int playerWidth;
	float playerVelocity;
	Circle mCollider;
	int frame;
	int OldTime;
	int FrameRate;
};
