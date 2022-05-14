#include "Enemy.h"

Enemy::Enemy(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file,gfx,pos, rot, scale)
{
}

void Enemy::lookat(vec3 lookat, vec3 offset)
{
	vec3 forward(0, 0, 1);
	vec3 up(0, 1, 0);
	vec3 TurretToPlayer = lookat - getPos();
	float xrot = vec3(TurretToPlayer.x, 0, TurretToPlayer.z).angle(forward);

	TurretToPlayer.Normalize();
	float yrot = acos(TurretToPlayer.Normalize() * up);

	this->setRot(vec3(yrot + offset.y, -xrot + offset.x, 0));
}


