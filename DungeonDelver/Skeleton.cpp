#include "Skeleton.h"
#include "Audio.h"
#include "Vector2.h"

Skeleton::Skeleton() {}

Skeleton::Skeleton(float x, float y, map <string, SDL_Texture*> skeletonTextureMap)
{
	this->mSkeletonPosition.x = x;
	this->mSkeletonPosition.y = y;
	this->SetSkeletonDirection(0, 0);
	// Set Character size
	skeletonHeight = 45;
	skeletonWidth = 30;
	skeletonVelocity = 70.0f;
	// Set Collider
	mCollider.x = mSkeletonPosition.x + (skeletonWidth / 2);
	mCollider.y = mSkeletonPosition.y + (skeletonHeight / 2);
	mCollider.radius = skeletonWidth / 2;

	// Set Line of Sight Circle
	lineOfSight.x = mSkeletonPosition.x + (skeletonWidth / 2);
	lineOfSight.y = mSkeletonPosition.y + (skeletonHeight / 2);
	lineOfSight.radius = 400.0f;

	// Take texture map and assign an array of sprites
	textureMap = skeletonTextureMap; 
	currentAnims[0] = skeletonTextureMap["down1"];
	currentAnims[1] = skeletonTextureMap["down1"];
	currentAnims[2] = skeletonTextureMap["down1"];

	// Used to prevent animations from iterating with the frame rate
	frame = 0;
	OldTime = 0;
	FrameRate = 100;
	// Set Audio
	audio = Audio();
	audio.InitializeCharacterAudio();
}

void Skeleton::SetSkeletonPosition(float x, float y)
{
	this->mSkeletonPosition.x = x;
	this->mSkeletonPosition.y = y;
}

Vector2 Skeleton::GetSkeletonPosition()
{
	return mSkeletonPosition;
}

void Skeleton::SetSkeletonDirection(float x, float y)
{
	this->mSkeletonDirection.x = x;
	this->mSkeletonDirection.y = y;
}

Vector2 Skeleton::GetSkeletonDirection()
{
	return mSkeletonDirection;
}

bool Skeleton::CheckCollision(Circle& enemyCollider)
{
	//Calculate total radius squared
	int totalRadiusSquared = mCollider.radius + enemyCollider.radius;

	//If the distance between the centers of the circles is less than the sum of their radii
	int x = mCollider.x - enemyCollider.x;
	int y = mCollider.y - enemyCollider.y;

	y *= y;
	x *= x;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	if ((x + y) < totalRadiusSquared) {
		return true;
	}

	return false;
}

Circle& Skeleton::GetCollider()
{
	return mCollider;
}

bool Skeleton::CanSeePlayer(Circle& playerCollider)
{
	//Calculate total radius squared
	int totalRadiusSquared = lineOfSight.radius + playerCollider.radius;

	//If the distance between the centers of the circles is less than the sum of their radii
	int x = playerCollider.x - mCollider.x;
	int y = playerCollider.y - mCollider.y;

	y *= y;
	x *= x;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	if ((x + y) < totalRadiusSquared) {
		return true;
	}

	return false;
}

void Skeleton::SetCurrentAnims(string direction) {
	// Depending on the input string direction - set the animation to specific images
	if (direction == "left") {
		currentAnims[0] = textureMap["left1"];
		currentAnims[1] = textureMap["left2"];
		currentAnims[2] = textureMap["left3"];
	}
	if (direction == "right") {
		currentAnims[0] = textureMap["right1"];
		currentAnims[1] = textureMap["right2"];
		currentAnims[2] = textureMap["right3"];
	}
	if (direction == "up") {
		currentAnims[0] = textureMap["up1"];
		currentAnims[1] = textureMap["up2"];
		currentAnims[2] = textureMap["up3"];
	}
	if (direction == "down") {
		currentAnims[0] = textureMap["down1"];
		currentAnims[1] = textureMap["down2"];
		currentAnims[2] = textureMap["down3"];
	}
	if (direction == "standing") {
		currentAnims[0] = currentAnims[1];
		currentAnims[1] = currentAnims[1];
		currentAnims[2] = currentAnims[1];
	}
}

void Skeleton::UpdateSkeleton(float deltaTime, Vector2 playerPosition)
{
	// Move Skeleton
	// Determine Skeleton Movement input
	SetSkeletonDirection(0, 0);
	int xMove = 0;
	int yMove = 0;

	Vector2 newVect = Vector2(playerPosition.x - mSkeletonPosition.x, playerPosition.y - mSkeletonPosition.y);
	newVect = newVect.Normalize();
	mSkeletonPosition.x += newVect.x;
	mSkeletonPosition.y += newVect.y;

	if (playerPosition.y < mSkeletonPosition.y) {
		yMove -= 1;
		SetCurrentAnims("up");
	}
	if (playerPosition.y > mSkeletonPosition.y) {
		yMove += 1;
		SetCurrentAnims("down");
	}
	if (playerPosition.x < mSkeletonPosition.x) {
		xMove -= 1;
		SetCurrentAnims("left");
	}
	if (playerPosition.x > mSkeletonPosition.x) {
		xMove += 1;
		SetCurrentAnims("right");
	}

	if (xMove != 1) {
		if (playerPosition.y < mSkeletonPosition.y) {
			SetCurrentAnims("up");
		}
		if (playerPosition.y > mSkeletonPosition.y) {
			SetCurrentAnims("down");
		}
	}
	if (yMove != 1) {
		if (playerPosition.x < mSkeletonPosition.x) {
			SetCurrentAnims("left");
		}
		if (playerPosition.x > mSkeletonPosition.x) {
			SetCurrentAnims("right");
		}
	}

	SetSkeletonDirection(xMove, yMove);

	// Update Skeleton Position
	if (GetSkeletonDirection().x != 0 || GetSkeletonDirection().y != 0)
	{
		mSkeletonPosition.x += GetSkeletonDirection().x * skeletonVelocity * deltaTime;
		mSkeletonPosition.y += GetSkeletonDirection().y * skeletonVelocity * deltaTime;
		if (!Mix_Playing(1)) {
			Mix_PlayChannel(1, audio.GetFootsteps(), 0);
		}
	}

	// Update Skeleton Position and Collider
	SetSkeletonPosition(mSkeletonPosition.x, mSkeletonPosition.y);
	mCollider.x = mSkeletonPosition.x + skeletonWidth / 2;
	mCollider.y = mSkeletonPosition.y + skeletonHeight / 2;
	// Update Line of Sight
	lineOfSight.x = mSkeletonPosition.x + skeletonWidth / 2;
	lineOfSight.y = mSkeletonPosition.y + skeletonHeight / 2;
}

void Skeleton::Respawn(float x, float y)
{
	while (isToClose) {
		int myX = rand() % 1500;
		int myY = rand() % 1000;
		int xDistance = myX - x;
		int yDistance = myY - y;
		int totalRadius = 300; 

		xDistance *= xDistance;
		yDistance *= yDistance;
		totalRadius *= totalRadius;

		if ((xDistance + yDistance) < totalRadius) {
			
		}
		else {
			SetSkeletonPosition(myX, myY);
			isToClose = false;
		}
	}
}

void Skeleton::RenderCharacter(SDL_Renderer* mRenderer)
{
	//Current animation frame
	// Keep same image 
	if (OldTime + FrameRate > SDL_GetTicks()) {
		SDL_RenderCopy(mRenderer, currentAnims[frame], NULL, new SDL_Rect{ (int)mSkeletonPosition.x,(int)mSkeletonPosition.y, skeletonWidth, skeletonHeight });
		return;
	}

	OldTime = SDL_GetTicks();
	SDL_RenderCopy(mRenderer, currentAnims[frame], NULL, new SDL_Rect{ (int)mSkeletonPosition.x,(int)mSkeletonPosition.y, skeletonWidth, skeletonHeight });

	frame++;

	if (frame == 3)
	{
		frame = 0;
	}
}

void Skeleton::DestroySkeleton()
{
	Mix_PlayChannel(-1, audio.GetSkeletonDestroy(), 0);
	audio.DestroyAudio();
	isToClose = true;
}
