#pragma once
#include "SFML/Audio.hpp"
#include "SFML/System.hpp"
#include <map>
#include <iostream>
#include "Vec.h"
#include <thread>

bool AnyAudio();

class SoundManager {
public:
	SoundManager();
	SoundManager(float volume);
	virtual ~SoundManager();
	void update(vec3 playerPos, vec3 playerDir, float dt = 0, vec3 upVec = vec3(0,1,0));
	void loadSound(std::string filePath, float volume = 1, std::string name = "");
	void playSound(std::string soundName);
	void playSound(std::string soundName, vec3 soundposition);
	void stopSound(std::string soundName);
	void setSoundVolume(std::string soundName, float volume);
	void setSoundPosition(std::string soundName, vec3 position);
	void setSoundOffset(std::string soundName, sf::Time time);
	void setLoopSound(std::string sound, bool loop);
	void updatePositionOfSound(vec3 position, std::string sound);
	void playMusic(std::string filePath, float volume = 1);
	void setMusicLoop(const bool loop);
	float getVolume()const;
	void changeMusic(std::string NewMusic, float volume, float timeToChange);
private:
	void changeVolumeMusics(float dt);
	struct soundPair {
		soundPair(std::string file, float volume);
		sf::Sound sound;
		sf::SoundBuffer soundBuffer;
	};
	std::map<std::string, soundPair*> sounds;
	sf::Music Music[2];
	int activeMusic;
	
	bool musicLoop;
	float volume;

	/*Changing musics vars*/
	bool changingMusic;
	float currentVolumeDiv[2];
	float toVolume;

	bool soundManagerActive;
};