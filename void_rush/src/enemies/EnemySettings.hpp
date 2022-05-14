#pragma once

struct Enemy_settings {
	int Turret_maxNrOfProjectiles = 5;
	float Turret_y_Offset = 1.f;
	float Turret_CD = 3.f;
	float Turret_range = 40.f;
	float Turret_scale = 0.5f;

	float Projectile_scale = 0.4f;

	float Spikes_y_Offset = 0.2f;
	float Spikes_timer = 2.f;
	float Spikes_scale = 0.0001f;
	
	float Snare_y_Offset = .5f;
	float Snare_timer = 3.f;
	float Snare_catchTimer = 0.1f;	
	float Snare_scale = 0.3f;
	float Snare_scale_y = 0.2f;
};
static Enemy_settings enemy_conf;