#include "SoundManager.h"
#include "Keyboard.h"

SoundManager::SoundManager()
{
	this->volume = 1;
	musicLoop = false;
	changingMusic = false;
	activeMusic = 0;
}

SoundManager::SoundManager(float volume)
{
	this->volume = volume;
	musicLoop = false;
	changingMusic = false;
	activeMusic = 0;
}

SoundManager::~SoundManager()
{
	for (auto const& [key, val] : sounds)
	{
		delete val.sound;
		delete val.soundBuffer;
	}
}

void SoundManager::update(vec3 playerPos, vec3 playerDir, float dt, vec3 upVec)
{
	sf::Listener::setPosition(playerPos.x, playerPos.y, playerPos.z);
	sf::Listener::setDirection(-playerDir.x, -playerDir.y, -playerDir.z);
	sf::Listener::setUpVector(upVec.x, upVec.y, upVec.z);
	if (changingMusic) {
		changeVolumeMusics(dt);
	}
}

void SoundManager::loadSound(std::string filePath, float volume, std::string name)
{
	soundPair sPair;
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	if (!buffer->loadFromFile(filePath)) {
		std::cout << "cannot load file:" << filePath << std::endl;
		return;
	}
	sf::Sound* newSound = new sf::Sound(*buffer);
	newSound->setVolume(this->volume * volume);
	
	std::string Soundname;
	if (name == "") {
		Soundname = filePath;
	}
	else {
		Soundname = name;
	}
	sPair.sound = newSound;
	sPair.soundBuffer = buffer;

	sounds.insert(std::make_pair(Soundname, sPair));
}

void SoundManager::playSound(std::string soundName)
{
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second.sound->play();
}

void SoundManager::playSound(std::string soundName, vec3 soundposition)
{
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second.sound->setPosition(soundposition.x, soundposition.y, soundposition.z);
	sounds.find(soundName)->second.sound->play();
}

void SoundManager::setLoopSound(std::string sound, bool loop)
{
	sounds.find(sound)->second.sound->setLoop(true);
}

void SoundManager::updatePositionOfSound(vec3 position, std::string sound)
{
	sounds.find(sound)->second.sound->setPosition(position.x, position.y, position.z);
}

void SoundManager::playMusic(std::string filePath, float volume)
{
	Music[activeMusic].setVolume(volume * this->volume);
	if (Music[activeMusic].openFromFile(filePath)) {
		Music[activeMusic].play();
		Music[activeMusic].setLoop(this->musicLoop);
	}
	else {
		std::cout << "cant find file: " << filePath << std::endl;
	}
}

void SoundManager::setMusicLoop(const bool loop)
{
	this->musicLoop = loop;
	Music[activeMusic].setLoop(loop);
}

float SoundManager::getVolume() const
{
	return this->volume;
}

void SoundManager::changeMusic(std::string NewMusic, float volume, float timeToChange)
{
	if (Music[(activeMusic + 1) % 2].openFromFile(NewMusic)) {
		Music[(activeMusic + 1) % 2].play();
		Music[(activeMusic + 1) % 2].setLoop(this->musicLoop);
		Music[(activeMusic + 1) % 2].setVolume(0);
	}
	else {
		std::cout << "cant find file: " << NewMusic << std::endl;
	}
	currentVolumeDiv[0] = Music[activeMusic].getVolume() / timeToChange;
	currentVolumeDiv[1] = volume / timeToChange;
	toVolume = volume;
	changingMusic = true;
	activeMusic++;
	activeMusic %= 2;
}

void SoundManager::changeVolumeMusics(float dt)
{
	Music[(activeMusic + 1) % 2].setVolume(Music[(activeMusic + 1) % 2].getVolume() - currentVolumeDiv[0] * dt);
	Music[activeMusic].setVolume(Music[activeMusic].getVolume() + currentVolumeDiv[1] * dt);
	if (Music[activeMusic].getVolume() >= toVolume) {
		Music[activeMusic].setVolume(toVolume);
		changingMusic = false;
		Music[(activeMusic + 1) % 2].stop();
	}
}

