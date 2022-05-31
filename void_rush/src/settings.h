#pragma once
#include <fstream>
#include <filesystem>


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
	settingsSingleTon() {
		//Check if folder exists
		char* t = new char[100];

		auto size = sizeof(t);
		_dupenv_s(&t, &size, "APPDATA");
		std::string path = std::string(t) + "\\void_rush";
		if (!std::filesystem::exists(path)) {
			std::filesystem::create_directory(path);
		}
		path = path + "\\Setting.data";
		if (!std::filesystem::exists(path)) {
			std::ofstream f(path);
			
			sett.volume = 1;
			sett.resolution = 4;
			sett.mouseSence = 0.6f;
			sett.fullscreen = true;
			//Saving settings
			std::ofstream output(path, std::ios::out | std::ios::binary);
			output.write((char*)&sett, sizeof(sett));
			output.close();

			f.close();
		}
		this->pathToSettingsFile = path;
	};
	static settingsSingleTon s_Instance;
	settings sett;
	std::string pathToSettingsFile;

};
