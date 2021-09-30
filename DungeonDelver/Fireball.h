#pragma once
#include "Character.h"
#include "Audio.h"
#include "Vector2.h"

class Fireball : public Character{
public:
	Fireball();
	Fireball(float x, float y, SDL_Texture* texture, Vector2 destination);
	SDL_Texture* GetTexture();

	void RenderCharacter(SDL_Renderer* mRenderer);
	void SetFireballDirection(int x, int y); 
	void SetFireballPosition(float x, float y);
	Vector2 GetFireballPosition();
	Vector2 GetFireballDirection();
	Vector2 GetFireballDestination();

	void UpdateFireball(float deltaTime);
	bool CheckCollision(Circle& enemyCollider);

	void DestroyFireball();

private:
	Vector2 mPosition;
	Vector2 mDirection;
	Vector2 mDestination;
	SDL_Texture* mTexture;
	Circle mCollider;
	Audio audio;

	int mHeight;
	int mWidth;
	float mVelocity;
};