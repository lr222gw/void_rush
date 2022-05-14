#pragma once

struct Enemy_settings {
	int Turret_maxNrOfProjectiles = 5;
	float Turret_y_Offset = 15.f;
	float Turret_CD = 2.f;
	float Turret_range = 40.f;

	float Spikes_y_Offset = 0.2f;
	float Spikes_timer = 2.f;
	
	float Snare_y_Offset = 0.0f;
	float Snare_timer = 3.f;
	float Snare_catchTimer = 0.1f;	
};
static Enemy_settings enemy_conf;