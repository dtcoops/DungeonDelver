#include "Fireball.h"
#include "Vector2.h"
#include <iostream>
using namespace std;

Fireball::Fireball()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mTexture = NULL;

	mWidth = 30;
	mHeight = 30;
	mVelocity = 0;

	// Set Collider
	mCollider.x = mPosition.x + (mWidth / 2);
	mCollider.y = mPosition.y + (mHeight / 2);
	mCollider.radius = 0;
}

Fireball::Fireball(float x, float y, SDL_Texture* texture, Vector2 destination)
{
	this->mPosition.x = x;
	this->mPosition.y = y;
	this->mTexture = texture;

	mWidth = 30;
	mHeight = 30;
	mVelocity = 100;
	mDestination = destination;

	// Set Collider
	mCollider.x = mPosition.x + (mWidth / 2);
	mCollider.y = mPosition.y + (mHeight / 2);
	mCollider.radius = mWidth / 2 + 5;

	audio.InitializeCharacterAudio();
	Mix_PlayChannel(-1, audio.GetFireballCreate(), 0);
}

SDL_Texture* Fireball::GetTexture()
{
	return mTexture;
}

void Fireball::SetFireballDirection(int x, int y) {
	this->mDirection.x = x;
	this->mDirection.y = y;
}

void Fireball::SetFireballPosition(float x, float y)
{

	this->mPosition.x = x;
	this->mPosition.y = y;
}

Vector2 Fireball::GetFireballPosition()
{
	return mPosition;
}

Vector2 Fireball::GetFireballDirection()
{
	return mDirection;
}

Vector2 Fireball::GetFireballDestination() 
{
	return mDestination;
}

void Fireball::UpdateFireball(float deltaTime)
{
	// Move Skeleton
	// Determine Skeleton Movement input
	SetFireballDirection(0, 0);

	Vector2 mDestination = GetFireballDestination();
		
	Vector2 newVect = Vector2(mDestination.x - mPosition.x, mDestination.y - mPosition.y);
	newVect = newVect.Normalize();
	mPosition.x += newVect.x;
	mPosition.y += newVect.y;
	
	// Update Skeleton Position and Collider
	SetFireballPosition(mPosition.x, mPosition.y);
	mCollider.x = mPosition.x + mWidth / 2;
	mCollider.y = mPosition.y + mHeight / 2;
	
	// Circle Object to collide with fireball at destination
	Circle explode = Circle();
	explode.x = mDestination.x;
	explode.y = mDestination.y;
	explode.radius = 8;

	if (CheckCollision(explode)) {
		DestroyFireball();
	}
}

bool Fireball::CheckCollision(Circle& target)
{
	// if objects position difference is less than the sum of their radii
	int totalRadiusSquared = mCollider.radius + target.radius;
	//If the distance between the centers of the circles is less than the sum of their radii
	int x = target.x - mCollider.x;
	int y = target.y - mCollider.y;
	// Square all values - never negative
	y *= y;
	x *= x;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;
	// If the distance between colliders is less than the combined radii - COLLISION
	if ((x + y) < totalRadiusSquared) {
		
		return true;
	}

	return false;
}

void Fireball::DestroyFireball()
{
	Mix_PlayChannel(-1, audio.GetFireballDestroy(), 0);
	mTexture = NULL;
	mCollider.x = 0;
	mCollider.y = 0;
	mCollider.radius = 0;
	audio.DestroyAudio();
}

void Fireball::RenderCharacter(SDL_Renderer* mRenderer)
{
	SDL_RenderCopy(mRenderer, mTexture, NULL, new SDL_Rect{ (int)mPosition.x,(int)mPosition.y, mWidth, mHeight });
}
