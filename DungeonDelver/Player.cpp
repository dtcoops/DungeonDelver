#include "Player.h"
#include "Vector2.h"

Player::Player(){}

Player::Player(float x, float y, map <string, SDL_Texture*> playerTextureMap)
{
	this->SetPlayerPosition(x, y);
	currentAnims[0] = textureMap["playerDown2"];
	currentAnims[1] = textureMap["playerDown2"];
	currentAnims[2] = textureMap["playerDown2"];
	this->SetPlayerDirection(0, 0);
	// Set Character size
	playerHeight = 45;
	playerWidth = 45;
	playerVelocity = 120.0f;
	// Set Collider
	mCollider.x = mPlayerPosition.x + (playerWidth / 2);
	mCollider.y = mPlayerPosition.y + (playerHeight / 2);
	mCollider.radius = playerWidth / 2;
	// Animations
	textureMap = playerTextureMap;
	frame = 0;
	OldTime = 0;
	FrameRate = 100;
	
	// Set Audio
	audio = Audio();
	audio.InitializeCharacterAudio();
}

void Player::SetPlayerPosition(float x, float y)
{
	this->mPlayerPosition.x = x;
	this->mPlayerPosition.y = y;
}

Vector2 Player::GetPlayerPosition()
{
	return mPlayerPosition;
}

void Player::SetPlayerDirection(float x, float y)
{
	this->mPlayerDirection.x = x;
	this->mPlayerDirection.y = y;
}

Vector2 Player::GetPlayerDirection()
{
	return mPlayerDirection;
}

Circle& Player::GetCollider()
{
	return mCollider;
}

void Player::GetInput(SDL_Event event, bool mIsRunning, int mScreenHeight, int mScreenWidth)
{
	SetPlayerDirection(0, 0);
	float x = 0;
	float y = 0;

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Move Player within screen constraints
	if (GetPlayerPosition().y <= 0) {
	}
	else {
		if (keyState[SDL_SCANCODE_W]) {
			y -= 1;
			SetCurrentAnims("up");
		}
	}
	if (GetPlayerPosition().y >= mScreenHeight - playerHeight) {
	}
	else {
		if (keyState[SDL_SCANCODE_S]) {
			y += 1;
			SetCurrentAnims("down");
		}
	}

	if (GetPlayerPosition().x <= 0) {
	}
	else {
		if (keyState[SDL_SCANCODE_A]) {
			x -= 1;
			SetCurrentAnims("left");
		}
	}
	if (GetPlayerPosition().x >= mScreenWidth - 300 - playerWidth) {
	}
	else {
		if (keyState[SDL_SCANCODE_D]) {
			x += 1;
			SetCurrentAnims("right");
		}
	}
	if (x == 0 && y == 0) {
		SetCurrentAnims("standing");
	}

	SetPlayerDirection(x, y);
}

bool Player::CheckCollision(Circle& enemyCollider)
{
	// if objects position difference is less than the sum of their radii
	int totalRadiusSquared = mCollider.radius + enemyCollider.radius;
	//If the distance between the centers of the circles is less than the sum of their radii
	int x = enemyCollider.x - mCollider.x;
	int y = enemyCollider.y - mCollider.y;
	// Square all values - never negative
	y *= y;
	x *= x;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;
	// If the distance between colliders is less than the combined radii - COLLISION
	if ((x + y) < totalRadiusSquared) {
		// Play defeat 
		Mix_PlayChannel(-1, audio.GetDefeat(), 0);
		return true;
	}

	return false;
}

void Player::UpdatePlayer(float deltaTime) {
	// Move Player
	Vector2 playerPosition = GetPlayerPosition();

	if (GetPlayerDirection().y != 0)
	{
		playerPosition.y += GetPlayerDirection().y * playerVelocity * deltaTime;
	}
	if (GetPlayerDirection().x != 0)
	{
		playerPosition.x += GetPlayerDirection().x * playerVelocity * deltaTime;
	}

	// Update Position and Collider
	SetPlayerPosition(playerPosition.x, playerPosition.y);
	mCollider.x = playerPosition.x + playerWidth / 2;
	mCollider.y = playerPosition.y + playerHeight / 2;
}

void Player::SetCurrentAnims(string direction) {
	if (direction == "left") {
		currentAnims[0] = textureMap["playerLeft1"]; 
		currentAnims[1] = textureMap["playerLeft2"]; 
		currentAnims[2] = textureMap["playerLeft3"];
	}
	if (direction == "right") {
		currentAnims[0] = textureMap["playerRight1"];
		currentAnims[1] = textureMap["playerRight2"];
		currentAnims[2] = textureMap["playerRight3"];
	}
	if (direction == "up") {
		currentAnims[0] = textureMap["playerUp1"];
		currentAnims[1] = textureMap["playerUp2"];
		currentAnims[2] = textureMap["playerUp3"];
	}
	if (direction == "down") {
		currentAnims[0] = textureMap["playerDown1"];
		currentAnims[1] = textureMap["playerDown2"];
		currentAnims[2] = textureMap["playerDown3"];
	}
	if (direction == "standing") {
		currentAnims[0] = currentAnims[1];
		currentAnims[1] = currentAnims[1];
		currentAnims[2] = currentAnims[1];
	}
	if (direction == "start") {
		currentAnims[0] = textureMap["playerUp2"];
		currentAnims[1] = textureMap["playerUp2"];
		currentAnims[2] = textureMap["playerUp2"];
	}
}

void Player::RenderCharacter(SDL_Renderer* mRenderer)
{
	//Current animation frame
	if (OldTime + FrameRate > SDL_GetTicks()) {
		SDL_RenderCopy(mRenderer, currentAnims[frame], NULL, new SDL_Rect{ (int)mPlayerPosition.x,(int)mPlayerPosition.y, playerHeight, playerWidth });
		return;
	}

	OldTime = SDL_GetTicks();
	SDL_RenderCopy(mRenderer, currentAnims[frame], NULL, new SDL_Rect{ (int)mPlayerPosition.x,(int)mPlayerPosition.y, playerHeight, playerWidth });

	frame++;

	if (frame == 3)
	{
		frame = 0;
	}
}

void Player::DestroyPlayer()
{
	// Destroy Audio
	audio.DestroyAudio();
	// Destroy Collider - Move off screen so it does not destroy anything else
	mCollider.x = 0;
	mCollider.y = 0;
	mCollider.radius = 0;
}
