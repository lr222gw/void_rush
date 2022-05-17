#include "SoundManager.h"
#include "Keyboard.h"
#include <mmdeviceapi.h>

bool hrIsOk(HRESULT hr) {
	if (hr != S_OK) {
		std::cout << "hr is not ok" << std::endl;
		return false;
	}
	return true;
}

bool AnyAudio()
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	HRESULT hr = S_OK;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDeviceCollection* pCollection = NULL;

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	if (!hrIsOk(hr)) {
		exit(1);
	}

	hr = pEnumerator->EnumAudioEndpoints(
		eRender, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,
		&pCollection);
	if (!hrIsOk(hr)) {
		exit(1);
	}

	UINT  count;
	hr = pCollection->GetCount(&count);
	if (!hrIsOk(hr)) {
		exit(1);
	}

	pEnumerator->Release();
	pCollection->Release();

	if (count == 0)
	{
		return false;
	}
	return true;
}

SoundManager::SoundManager()
{
	soundManagerActive = true;
	if (!AnyAudio()) {
		soundManagerActive = false;
	}
	this->volume = 1;
	musicLoop = false;
	changingMusic = false;
	activeMusic = 0;
}

SoundManager::SoundManager(float volume)
{
	soundManagerActive = true;
	if (!AnyAudio()) {
		soundManagerActive = false;
	}
	this->volume = volume;
	musicLoop = false;
	changingMusic = false;
	activeMusic = 0;
}

SoundManager::~SoundManager()
{
	for (auto const& [key, val] : sounds)
	{
		delete val;
	}
}

void SoundManager::update(vec3 playerPos, vec3 playerDir, float dt, vec3 upVec)
{
	if (!soundManagerActive) {
		return;
	}
	sf::Listener::setPosition(playerPos.x, playerPos.y, playerPos.z);
	sf::Listener::setDirection(-playerDir.x, -playerDir.y, -playerDir.z);
	sf::Listener::setUpVector(upVec.x, upVec.y, upVec.z);
	if (changingMusic) {
		changeVolumeMusics(dt);
	}
}

void SoundManager::loadSound(std::string filePath, float volume, std::string name)
{	
	if (!soundManagerActive) {
		return;
	}
	std::string Soundname;
	if (name == "") {
		Soundname = filePath;
	}
	else {
		Soundname = name;
	}

	sounds.insert(std::make_pair(Soundname, new soundPair(filePath, this->volume * volume)));
}

void SoundManager::playSound(std::string soundName)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.play();
}

void SoundManager::playSound(std::string soundName, vec3 soundposition)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.setPosition(soundposition.x, soundposition.y, soundposition.z);
	sounds.find(soundName)->second->sound.play();
}

void SoundManager::stopSound(std::string soundName)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.stop();
}

void SoundManager::setSoundVolume(std::string soundName, float volume)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.setVolume(volume);
}

void SoundManager::setSoundPosition(std::string soundName, vec3 position)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.setPosition(sf::Vector3(position.x, position.y, position.z));
}

void SoundManager::setSoundOffset(std::string soundName, sf::Time time)
{
	if (!soundManagerActive) {
		return;
	}
	if (sounds.find(soundName) == sounds.end()) {
		std::cout << "couldn't find sound: " << soundName << std::endl;
		return;
	}
	sounds.find(soundName)->second->sound.setPlayingOffset(time);
}

void SoundManager::setLoopSound(std::string sound, bool loop)
{
	if (!soundManagerActive) {
		return;
	}
	sounds.find(sound)->second->sound.setLoop(true);
}

void SoundManager::updatePositionOfSound(vec3 position, std::string sound)
{
	if (!soundManagerActive) {
		return;
	}
	sounds.find(sound)->second->sound.setPosition(position.x, position.y, position.z);
}

void SoundManager::playMusic(std::string filePath, float volume)
{
	if (!soundManagerActive) {
		return;
	}
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
	if (!soundManagerActive) {
		return;
	}
	this->musicLoop = loop;
	Music[activeMusic].setLoop(loop);
}

float SoundManager::getVolume() const
{
	return this->volume;
}

void SoundManager::changeMusic(std::string NewMusic, float volume, float timeToChange)
{
	if (!soundManagerActive) {
		return;
	}
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

SoundManager::soundPair::soundPair(std::string file, float volume)
{
	if (!soundBuffer.loadFromFile(file)) {
		std::cout << "cannot load file:" << file << std::endl;
		return;
	}
	sound.setBuffer(soundBuffer);
	sound.setVolume(volume);
}


