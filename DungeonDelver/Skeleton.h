#pragma once
#include "Character.h"
#include "Audio.h"
#include "Vector2.h"

#include <string>
#include <map>
#include <iostream>
using namespace std;

class Skeleton : public Character
{
public:
	Skeleton();
	Skeleton(float x, float y, map<string, SDL_Texture*> skeletonTextureMap);

	void SetSkeletonPosition(float x, float y);
	Vector2 GetSkeletonPosition();
	void SetSkeletonDirection(float x, float y);
	Vector2 GetSkeletonDirection();

	bool CheckCollision(Circle& enemyCollider);
	bool CanSeePlayer(Circle& enemyCollider);

	void SetCurrentAnims(string direction);

	Circle& GetCollider();
	void UpdateSkeleton(float deltaTime, Vector2 playerPosition);
	void Respawn(float x, float y);

	void RenderCharacter(SDL_Renderer* mRenderer);
	void DestroySkeleton();


	bool isToClose = false;
private:
	Vector2 mSkeletonPosition;
	Vector2 mSkeletonDirection;
	map <string, SDL_Texture*> textureMap;
	SDL_Texture* currentAnims[3];
	Circle mCollider;
	Circle lineOfSight;
	Audio audio;

	bool canSeePlayer;
	int skeletonHeight;
	int skeletonWidth;
	float skeletonVelocity;

	int frame;
	int OldTime;
	int FrameRate;
};
