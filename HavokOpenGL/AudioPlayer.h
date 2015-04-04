#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <iostream>
#include <string>
#include <Windows.h>
#include "irrKlang/irrKlang.h"

using namespace irrklang;

class AudioPlayer
{
private:
	ISoundEngine* sndEngine;
	ISound* currSound;
	std::string fileName;
	int trackPos;
	float volume;
public:
	AudioPlayer(void);
	AudioPlayer(std::string soundFile); //initialises the file sound, doesn't play it
	~AudioPlayer(void);

	void playSound();
	void playSound(std::string soundFile);
	void pause();
	void resume();
	void volUp();
	void volDown();
	void mute();
};

#endif

