#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <vector>
#include "object.h"
#include "Light.h"
#include <string>
#include "enemies/EnemySettings.hpp"

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
	void render_ghost_widgets();
	void render_player_widgets();
	void render_physics_widgets();
	void render_enemy_widgets();
	void render_debuginfo_widgets();
	void set_owner(Game* game);
private:
	std::vector<object*> obj;
	std::vector<Light*> light;
	Game* owner;
};