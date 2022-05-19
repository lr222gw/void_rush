#pragma once
#include <fstream>

struct settings {
	int volume;
	float mouseSence;
	int resolution = 0;
	bool fullscreen = false;
};

static int resolutions[5][2]{
	{640, 360},
	{800, 600},
	{1280, 720},
	{1600, 900},
	{1920, 1080}
};

class settingsSingleTon {
public:
	static settingsSingleTon& GetInst();
	void readSettings();
	void setSettings(settings sett);
	settings& getSettings();

	settingsSingleTon(settingsSingleTon const&) = delete;
	void operator=(settingsSingleTon const&) = delete;
private:
	settingsSingleTon() {};
	static settingsSingleTon s_Instance;
	settings sett;

};
