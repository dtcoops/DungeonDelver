#include "Audio.h"

Audio::Audio()
{
	backgroundMusic = NULL;
	skeletonDestroy = NULL;
	skeletonShout = NULL;
	fireballDestroy = NULL;
	fireballCreate = NULL;
	footsteps = NULL;
	gameOver = NULL;
}

void Audio::InitializeAudio()
{
	Mix_OpenAudio(
		44100,
		MIX_DEFAULT_FORMAT,
		2,
		2048
	);
	backgroundMusic = Mix_LoadMUS("Assets\\Audio\\8-A-la-poursuite-dune-ombre-du-p.wav");
	Mix_Volume(2, 50);
	Mix_PlayMusic(backgroundMusic, -1);
}

void Audio::InitializeCharacterAudio()
{
	Mix_OpenAudio(
		44100,
		MIX_DEFAULT_FORMAT,
		2,
		2048
	);

	footsteps = Mix_LoadWAV("Assets\\Audio\\443614__hachiman935__pasos-02.wav");
	fireballCreate = Mix_LoadWAV("Assets\\Audio\\334237__liamg-sfx__fireball-cast-3.wav");
	fireballDestroy = Mix_LoadWAV("Assets\\Audio\\431174__highpixel__fireball-explosion.wav");
	gameOver = Mix_LoadWAV("Assets\\Audio\\277403__landlucky__game-over-sfx-and-voice.wav");
	skeletonDestroy = Mix_LoadWAV("Assets\\Audio\\381859__cribbler__skeleton-bones-game.wav");
	// Set Volume
	Mix_VolumeChunk(footsteps, 60); // mute a little or its all you hear
	Mix_VolumeChunk(gameOver, 50); // 100% is terrifying
}

void Audio::DestroyAudio()
{
	backgroundMusic = NULL;
	skeletonDestroy = NULL;
	skeletonShout = NULL;
	fireballDestroy = NULL;
	fireballCreate = NULL;
	footsteps = NULL;
	gameOver = NULL;
	Mix_Quit();
}

void Audio::PauseMusic()
{
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(backgroundMusic, -1);
	}
	else {
		if (Mix_PausedMusic() == 1) {
			Mix_ResumeMusic();
		}
		else {
			Mix_PauseMusic();
		}
	}
}

Mix_Chunk* Audio::GetFootsteps() {
	return footsteps;
}

void Audio::StopFootsteps() {
	footsteps = NULL;
}

Mix_Chunk* Audio::GetFireballCreate() {
	return fireballCreate;
}

Mix_Chunk* Audio::GetFireballDestroy() {
	return fireballDestroy;
}

Mix_Chunk* Audio::GetDefeat() {
	return gameOver;
}

Mix_Chunk* Audio::GetSkeletonDestroy() {
	return skeletonDestroy;
}