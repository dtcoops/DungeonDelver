#pragma once
#include "SDL/SDL_mixer.h"
class Audio
{
public:
	Audio();

	void InitializeAudio();
	void InitializeCharacterAudio();
	void DestroyAudio();
	void PauseMusic();
	Mix_Chunk* GetFootsteps();
	void StopFootsteps();
	Mix_Chunk* GetFireballCreate();
	Mix_Chunk* GetFireballDestroy();
	Mix_Chunk* GetDefeat();
	Mix_Chunk* GetSkeletonDestroy();
private:
	Mix_Music* backgroundMusic;
	Mix_Chunk* skeletonDestroy;
	Mix_Chunk* skeletonShout;
	Mix_Chunk* fireballDestroy;
	Mix_Chunk* fireballCreate;
	Mix_Chunk* footsteps;
	Mix_Chunk* gameOver;
};
