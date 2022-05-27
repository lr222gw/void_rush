#include "settings.h"

settingsSingleTon& settingsSingleTon::GetInst()
{
    return s_Instance;
}

void settingsSingleTon::readSettings()
{
	std::ifstream input(this->pathToSettingsFile, std::ios::out | std::ios::binary);
	if (!input) {
		sett.volume = 1;
		sett.resolution = 4;
		sett.mouseSence = 0.6f;
	}
	input.read((char*)&sett, sizeof(sett));
	input.close();
	settingsSingleTon::GetInst().setSettings(sett);
}

void settingsSingleTon::setSettings(settings sett)
{
    this->sett = sett;
}

settings& settingsSingleTon::getSettings()
{
    return sett;
}


settingsSingleTon settingsSingleTon::s_Instance;