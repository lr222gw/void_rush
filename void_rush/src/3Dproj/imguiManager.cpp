#include "imguiManager.h"
#include "Game.h"

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}

void ImguiManager::takeObject(object* obj)
{
	this->obj.push_back(obj);
}

void ImguiManager::takeLight(Light* light)
{
	this->light.push_back(light);
}

void ImguiManager::updateRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (owner != nullptr) {
		render_player_widgets();
		render_generation_widgets();
		update_lights(owner->lightNr);
		render_physics_widgets();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
void ImguiManager::update_lights(int lightNr)
{
	
	static auto vec = std::vector<int>(0);
	//for (int i = 0; i < vec.size(); i++) {
	//	std::string name = "obj " + std::to_string(i);
	//	if (ImGui::Begin(name.c_str())) {
	//		ImGui::SliderFloat("Xpos", &owner->obj[vec[i]]->getxPos(), 40.0f, -40.0f);
	//		ImGui::SliderFloat("Ypos", &owner->obj[vec[i]]->getyPos(), 40.0f, -40.0f);
	//		ImGui::SliderFloat("Zpos", &owner->obj[vec[i]]->getzPos(), 40.0f, -40.0f);
	//		ImGui::SliderFloat("XRot", &owner->obj[vec[i]]->getxRot(), 20.0f, -20.0f);
	//		ImGui::SliderFloat("YRot", &owner->obj[vec[i]]->getyRot(), 20.0f, -20.0f);
	//		ImGui::SliderFloat("ZRot", &owner->obj[vec[i]]->getzRot(), 20.0f, -20.0f);
	//	}
	//	ImGui::End();
	//}
	if (light.size() > 0) {
		std::string name = "light" + std::to_string(lightNr);
		if (ImGui::Begin(name.c_str())) {
			ImGui::SliderFloat("Xpos", &light[lightNr]->getPos().x, 60.0f, -60.0f);
			ImGui::SliderFloat("Ypos", &light[lightNr]->getPos().y, 60.0f, -60.0f);
			ImGui::SliderFloat("Zpos", &light[lightNr]->getPos().z, 60.0f, -60.0f);
			ImGui::SliderFloat("XRot", &light[lightNr]->getRotation().x, 6.3f, -6.3f);
			ImGui::SliderFloat("YRot", &light[lightNr]->getRotation().y, 6.3f, -6.3f);
			ImGui::SliderFloat("ZRot", &light[lightNr]->getRotation().z, 6.3f, -6.3f);
		}
		ImGui::End();
	}
	
}

void ImguiManager::render_generation_widgets()
{
		
	static std::string name = "Generation";
	if (ImGui::Begin(name.c_str())) {
		
		ImGui::InputInt("Seed", &owner->generationManager->seed);
		if(ImGui::TreeNode("Player_jump_checker")){
		
			ImGui::InputFloat("gravity", &owner->generationManager->player_jump_checker->gravity);
			ImGui::InputFloat("jump velocity", &owner->generationManager->player_jump_checker->jumpvel);
			ImGui::InputFloat("launch angle", &owner->generationManager->player_jump_checker->launchangle);
			
			float* pos[3] = { &owner->generationManager->player_jump_checker->pos.x,
				&owner->generationManager->player_jump_checker->pos.y,
				&owner->generationManager->player_jump_checker->pos.z };
			ImGui::InputFloat3("position", *pos);

			ImGui::InputFloat("speed", &owner->generationManager->player_jump_checker->speed);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Position_generator")) {

			ImGui::InputInt("Number of platforms", &owner->generationManager->position_gen->elements);						
			
			ImGui::TreePop();
		}		

		if (ImGui::TreeNode("Generation_manager")) {			

			if (ImGui::TreeNode("Platforms")) {

				for (int i = 0; i < owner->generationManager->platformObjs.size(); i++) {

					std::string name = "platform:" + std::to_string(i);
					
					float* pos[3] = { &owner->generationManager->platformObjs[i]->pos.x,
							&owner->generationManager->platformObjs[i]->pos.y,
							&owner->generationManager->platformObjs[i]->pos.z };

					ImGui::InputFloat3(name.c_str(), *pos);
				}				
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if(ImGui::Button("initialize")){
			owner->generationManager->initialize();
			owner->generationManager->generateGraph();
		}
	}
	ImGui::End();

}

void ImguiManager::render_physics_widgets()
{
	static std::string name = "Physics";
	if (ImGui::Begin(name.c_str())) {
		//owner->player->speed
							
		float* init_speed[3] = {&owner->player->speed.x, &owner->player->speed.y, &owner->player->speed.z};
		float min_speed = 0; 
		float max_speed = 304;
		static float speedSlider = owner->player->speed.x;
		//ImGui::SliderFloat3("Speed", *init_speed, min_speed, max_speed);
		if (ImGui::SliderFloat("Speed", &speedSlider, min_speed, max_speed)){
			*init_speed[0] = speedSlider;
			*init_speed[1] = speedSlider;
			*init_speed[2] = speedSlider;
		}
		ImGui::InputFloat("jumpSpeed", &owner->player->jumpSpeed.y);
		ImGui::InputFloat("Gravity", &owner->player->gravity.y);
			
			

	}
	ImGui::End();
}

void ImguiManager::render_player_widgets()
{
	std::string name = "Player";

	if (ImGui::Begin(name.c_str())) {
		
		if (ImGui::Checkbox("noClip", &owner->player->noClip)) {
			owner->player->grounded = true;
		}
		float* pos[3] = { &owner->player->pos.x,
			&owner->player->pos.y,
			&owner->player->pos.z };
		ImGui::InputFloat3("PlayerPos", *pos);
		
		
		
	}
	ImGui::End();
}

void ImguiManager::set_owner(Game* game)
{
	this->owner = game;
}

