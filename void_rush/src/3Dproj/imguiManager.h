#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <vector>
#include "object.h"
#include "Light.h"
#include <string>

class Game;
class ImguiManager {
public:
	ImguiManager();
	~ImguiManager();
	void takeObject(object* obj);
	void takeLight(Light* light);
	void updateRender();
	void update_lights(int lightNr);
	void render_generation_widgets();
	void render_player_widgets();
	void render_physics_widgets();
	void set_owner(App* game);
private:
	std::vector<object*> obj;
	std::vector<Light*> light;
	Game* owner;
};