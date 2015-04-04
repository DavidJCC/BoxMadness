#include "AudioPlayer.h"
#include <cstdlib>

AudioPlayer::AudioPlayer(void)
{
	sndEngine = createIrrKlangDevice(); //creates the sound engine

	if(!sndEngine)  //check the sound engine been created
		std::cout << "Sound engine could not be created" << std::endl;

	currSound = 0;
	fileName = "";
	volume = 100.0f;
}

AudioPlayer::AudioPlayer(std::string soundFile)
{
	sndEngine = createIrrKlangDevice(); //creates the sound engine
	
	if(!sndEngine) //check the sound engine been created
		std::cout << "Sound engine could not be created" << std::endl;
	
	fileName = soundFile;
	currSound = 0;
	volume = 100.0f;
}

AudioPlayer::~AudioPlayer(void)
{
	sndEngine->drop();
	if(currSound)
		currSound->drop();
}

void AudioPlayer::playSound()
{
	currSound = sndEngine->play2D(fileName.c_str(), false, false, true);
}


void AudioPlayer::playSound(std::string soundFile)
{
	if(soundFile == "")
		std::cout << "File isn't recognised" << std::endl;

	fileName = soundFile;
	currSound = sndEngine->play2D(soundFile.c_str(), false, false, true);
}

void AudioPlayer::pause()
{
	trackPos = currSound->getPlayPosition();
	sndEngine->setAllSoundsPaused();
}

void AudioPlayer::resume()
{
	currSound = sndEngine->play2D(fileName.c_str(), false, false, true);

	if(trackPos != -1)
		currSound->setPlayPosition(trackPos);
}

void AudioPlayer::volUp()
{
	volume += 10;
	
	if(volume > 100)
		volume = 100;
	currSound->setVolume(volume/100.0f);
}

void AudioPlayer::volDown()
{
	volume -= 10.0f;
	
	if(volume < 0)
		volume = 0;
	currSound->setVolume(volume/100.0f);
}

void AudioPlayer::mute()
{
	if(volume > 0)
		currSound->setVolume(0);
	else
		currSound->setVolume(volume);
}