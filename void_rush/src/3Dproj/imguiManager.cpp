#include "imguiManager.h"
#include "Game.h"

static vec3 prev_player_speed;

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
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (owner != nullptr) {
		render_player_widgets();
		render_generation_widgets();
		update_lights(owner->lightNr);
		render_physics_widgets();
		render_ghost_widgets();
		render_debuginfo_widgets();
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
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	static std::string name = "Generation";
	if (ImGui::Begin(name.c_str())) {
		
		ImGui::InputInt("Seed", &owner->generationManager->seed);
		if(ImGui::TreeNode("test_seeds")){
			static int seed_from_this_run = owner->generationManager->seed;
			if (ImGui::SmallButton("Use_this_run_seed")) {
				owner->generationManager->seed = seed_from_this_run;
			}
			if(ImGui::SmallButton("Use_1650807068")){
				owner->generationManager->seed = 1650807068;
			}
			if (ImGui::SmallButton("Use_123")) {
				owner->generationManager->seed = 123;
			}
			ImGui::TreePop();
		}
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

			if (ImGui::TreeNode("Jumppoints")) {
				
				ImGui::InputFloat("random_dist_dividier", &owner->generationManager->position_gen->JP_conf.random_dist_dividier);
				ImGui::InputFloat("y_min_clamp", &owner->generationManager->position_gen->JP_conf.y_min_clamp);
				ImGui::InputFloat("y_max_clamp", &owner->generationManager->position_gen->JP_conf.y_max_clamp);
				ImGui::InputFloat("rand_dir_min_angle_percent", &owner->generationManager->position_gen->JP_conf.rand_dir_min_angle_percent);
				ImGui::InputFloat("rand_dir_max_angle_percent", &owner->generationManager->position_gen->JP_conf.rand_dir_max_angle_percent);
				ImGui::TreePop();
			} 
			if (ImGui::TreeNode("Anchors")) {
				ImGui::InputInt("Number of Anchors", &owner->generationManager->position_gen->AP_conf.nrOfAnchors);
				ImGui::InputFloat("minStepMod", &owner->generationManager->position_gen->AP_conf.minStepMod);
				ImGui::InputFloat("stepMax", &owner->generationManager->position_gen->AP_conf.stepMax);
				ImGui::InputFloat("stepMaxHeight", &owner->generationManager->position_gen->AP_conf.stepMaxHeight);
				ImGui::InputFloat("stepMinHeight", &owner->generationManager->position_gen->AP_conf.stepMinHeight);
				ImGui::InputFloat("lowest_Height", &owner->generationManager->position_gen->AP_conf.lowest_Height);
				ImGui::InputFloat("minZAngle", &owner->generationManager->position_gen->AP_conf.minZAngle);				
				ImGui::InputFloat("spawn_Y_offset_origo", &owner->generationManager->position_gen->AP_conf.spawn_Y_offset_origo);
				ImGui::InputFloat("freeFallModifier", &owner->generationManager->position_gen->AP_conf.freeFallModifier);
				ImGui::InputInt("freeFallRate", &owner->generationManager->position_gen->AP_conf.freeFallRate);
				ImGui::TreePop(); 
			}
			if (ImGui::TreeNode("Platforms,Shape")) {
				float* scale[3] = { &Shape::shape_conf.default_scale.x,
					& Shape::shape_conf.default_scale.y,
					& Shape::shape_conf.default_scale.z };
				
				ImGui::InputFloat3("default_scale", *scale);
				ImGui::InputInt("maxNrOfVoxels_AP", &Shape::shape_conf.maxNrOfVoxels_AP);
				ImGui::InputInt("minNrOfVoxels_AP", &Shape::shape_conf.minNrOfVoxels_AP);
				ImGui::InputInt("maxNrOfVoxels_JP", &Shape::shape_conf.maxNrOfVoxels_JP);
				ImGui::InputInt("minNrOfVoxels_JP", &Shape::shape_conf.minNrOfVoxels_JP);
				ImGui::InputFloat("scaleAnchor_XY_Factor", &Shape::shape_conf.scaleAnchor_XY_Factor);
				ImGui::InputFloat("plattform_voxel_margin", &Shape::shape_conf.plattform_voxel_margin);
				ImGui::SliderInt("distance_padding", &Shape::shape_conf.max_clamp_padding, -10,10);
				ImGui::Checkbox("tryRandom", &Shape::shape_conf.tryRandom);
				ImGui::SliderInt("randomOccurances", &Shape::shape_conf.randomOccurances, 1, 10);

				ImGui::TreePop();
			}
			
			ImGui::TreePop();
		}		

		if (ImGui::TreeNode("Generation_manager")) {			

			if (owner->collisionHandler.lastCollided_ShapePlatform) {
				std::string info = std::to_string(owner->collisionHandler.lastCollided_ShapePlatform->index).c_str();
				ImGui::Text(("Current Platform.shape index: " + info ).c_str());
			}

			if (ImGui::TreeNode("Platforms")) {

				

				auto anchors = owner->generationManager->position_gen->getAnchors();
				auto jumpPoints = owner->generationManager->position_gen->getJumpPoints();
				Platform* current_anchor = anchors->at(0);
				Platform* current_jump = jumpPoints->at(0);
				int shape_index = 0;
				while (current_anchor) {
					shape_index = current_anchor->platformShape.index;

					std::string name = "A_platform_[" + std::to_string(shape_index) + "]:";
					std::string name_pos = "A_platform_["+ std::to_string(shape_index) +"]_pos:" ;					

					float* pos[3] = { &current_anchor->pos.x,
							&current_anchor->pos.y,
							&current_anchor->pos.z };

					ImGui::DragFloat3(name_pos.c_str(), *pos);

					if (ImGui::TreeNode(name.c_str())) {
						int voxel_index = 0;
						for (Center_Index_Pair vox : current_anchor->platformShape.previousVoxels) {
							std::string vox_id = "Voxel[" + std::to_string(voxel_index) + "]:";

							float* voxel_pos[3] = { &vox.current_center.x,
							&vox.current_center.y,
							&vox.current_center.z };

							ImGui::DragFloat3(name_pos.c_str(), *voxel_pos);

							voxel_index++;
						}

						ImGui::TreePop();
					}

					current_anchor = current_anchor->next;
				}
				while (current_jump) {
					shape_index = current_jump->platformShape.index;

					std::string name = "J_platform_[" + std::to_string(shape_index) + "]:";					
					std::string name_pos = "J_platform_["+ std::to_string(shape_index) +"]_pos:" ;

					float* pos[3] = { &current_jump->pos.x,
							&current_jump->pos.y,
							&current_jump->pos.z };

					ImGui::DragFloat3(name_pos.c_str(), *pos);

					if (ImGui::TreeNode(name.c_str())) {
						int voxel_index = 0;
						for(Center_Index_Pair vox : current_jump->platformShape.previousVoxels){
							std::string vox_id = "Voxel[" + std::to_string(voxel_index) + "]:";
							
							float* voxel_pos[3] = { &vox.current_center.x,
							&vox.current_center.y,
							&vox.current_center.z };
							
								ImGui::DragFloat3(name_pos.c_str(), *voxel_pos);

							voxel_index++;
						}

						ImGui::TreePop();
					}
					
					current_jump = current_jump->next;
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::Checkbox("init from Origo", &owner->generationManager->position_gen->imgui_conf.useOrigo);
		

		if(ImGui::Button("initialize")){
			auto temp = owner->player->speed;	//Fix to use initialize while noclipping
			static vec3 empty;
			if (empty == prev_player_speed){
				prev_player_speed = owner->player->speed;
			}
			owner->player->speed = prev_player_speed;  
			owner->generationManager->initialize();
			owner->generationManager->generateGraph();
			owner->player->speed = temp;
		}

		if (ImGui::Button("ExportFirstShape")) {
			owner->generationManager->shape_export->export_final_model("map");
			
		}
	}
	ImGui::End();

}

void ImguiManager::render_ghost_widgets()
{
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	std::string name = "Ghost";
	if (ImGui::Begin(name.c_str())) {
		
		ImGui::Checkbox("Activate Ghost", &owner->ghost->active);
		static float min_speed = 0;
		static float max_speed = 10;
		ImGui::SliderFloat("Speed", &owner->ghost->speed, min_speed, max_speed);
		ImGui::InputFloat("Point Limit", &owner->ghost->rangeToPointBeforeNewPoint);
		ImGui::InputFloat("Player range Limit", &owner->ghost->rangeToPlayerBeforeNearestWay);
		ImGui::InputFloat("Speed Increase", &owner->ghost->speed_increase);
		ImGui::InputFloat("time_s interval", &owner->ghost->ghost_Time_interval);
		float* init_force[3] = { &owner->ghost->force.x, &owner->ghost->force.y, &owner->ghost->force.z };
		static float min_force = 0;
		static float max_force = 304;
		static float forceSlider = owner->ghost->force.x;
		if (ImGui::SliderFloat("Force", &forceSlider, min_force, max_force)) {
			*init_force[0] = forceSlider;
			//*init_force[1] = forceSlider;
			*init_force[2] = forceSlider;
		}
		if (ImGui::Button("Reset", ImVec2(100, 25)))
		{
			owner->ghost->Reset();
		}
	}
	ImGui::End();
}

void ImguiManager::render_physics_widgets()
{
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	static std::string name = "Physics";
	if (ImGui::Begin(name.c_str())) {
		//owner->player->speed
							
		float* init_speed[3] = {&owner->player->speed.x, &owner->player->speed.y, &owner->player->speed.z};
		static float min_speed = 0; 
		static float max_speed = 100;
		static float speedSlider = owner->player->speed.x;
		//ImGui::SliderFloat3("Speed", *init_speed, min_speed, max_speed);
		if (ImGui::SliderFloat("Speed", &speedSlider, min_speed, max_speed)){
			*init_speed[0] = speedSlider;
			*init_speed[1] = speedSlider;
			*init_speed[2] = speedSlider;
		}
		ImGui::InputFloat("Gravity", &owner->player->gravity.y);
		ImGui::InputFloat("Jumpforce", &owner->player->jumpForce);
		static float min_MidAdj = 1;
		static float max_MidAdj = 10;
		ImGui::SliderFloat("Air adjustment", &owner->player->midAirAdj, min_MidAdj, max_MidAdj);
		ImGui::InputFloat("Lives", &owner->player->health);
	}
	ImGui::End();
}

void ImguiManager::render_debuginfo_widgets()
{
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	if (ImGui::Begin("Debuginfo")) {

		static auto cur_time = ImGui::GetTime();
		static auto time_offset = ImGui::GetTime();
		static auto prev_time = ImGui::GetTime() - time_offset;
		static auto past_time = 0;
		static auto frame_count = 0;
		static float fps;
		static float average_fps = 0;
		static float allowed_diviation_from_avg = 5;
		static float update_interval = 1;

		cur_time = ImGui::GetTime() ;
		if ((prev_time + update_interval) < (cur_time)) {
			fps = -(frame_count - (float)ImGui::GetFrameCount()) / update_interval;
			frame_count = ImGui::GetFrameCount() ;
			past_time = (int)(cur_time - time_offset);
			average_fps = (float)(frame_count /(past_time == 0 ? 1 : past_time));
			prev_time = cur_time;
		}

		ImGui::InputFloat("FPS", &fps);
		ImGui::InputFloat("avg. FPS", &average_fps);
		if(ImGui::TreeNode("settings")){
			ImGui::SliderFloat("update_interval", &update_interval, 0.1f, 10.f);
			ImGui::SliderFloat("allowed_diviation", &allowed_diviation_from_avg, 0.1f, 20.f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ImguiManager::render_player_widgets()
{
	if (!(DEVMODE_ || DEBUGMODE))
		return;

	std::string name = "Player";
	if (ImGui::Begin(name.c_str())) {
		static bool player_invincible = false;
		
		ImGui::Checkbox("Alive", &owner->player->alive);
		if (ImGui::Button("Toggle noClip")) {
			owner->player->noClip = owner->player->noClip ? false : true;
			if(!owner->player->noClip){
				owner->player->speed = prev_player_speed;
				//owner->player->grounded = false;
			}else{
				prev_player_speed = owner->player->speed;
				owner->player->grounded = true;
				owner->player->velocity = vec3(0.f, 0.f, 0.f);
				owner->player->speed = vec3(15.f, 15.f, 15.f);
			}			
		}

		ImGui::Checkbox("Invincible", &owner->player->invincible);

		float* pos[3] = { &owner->player->pos.x,
			&owner->player->pos.y,
			&owner->player->pos.z };
		
		ImGui::InputFloat3("PlayerPos", *pos);	

		float* vel[3] = { &owner->player->velocity.x,
							& owner->player->velocity.y,
							& owner->player->velocity.z };

		ImGui::InputFloat3("vel", *vel);
		ImGui::InputFloat("jumpForce", &owner->player->jumpForce);
		ImGui::InputFloat("maxDepth", &owner->player->maxDepth);

		float* scale[3] = { &owner->player->scale.x,
							&owner->player->scale.y,
							&owner->player->scale.z };
		if(ImGui::InputFloat3("Scale", *scale)){
			owner->player->setBoundingBox(DirectX::XMFLOAT3(0, -owner->player->scale.y - 0.01f, 0), DirectX::XMFLOAT3(owner->player->scale.x - 0.01f, 0.10f, owner->player->scale.z - 0.01f));
		}
	}
	ImGui::End();
}

void ImguiManager::set_owner(Game* game)
{
	this->owner = game;
}

