#pragma once

#include <fstream>
#include <iostream>

#include "../common/Helper.hpp"


struct test
{
    float time = 0;
    float t1, t2;
    float deltlaT = 1;

    bool grounded = false;

    float mass = 50.0f;
    float grav_a = -9.82f;
    float start_v = 60.0f;
    float jmp_spd = 20.0f;
    float friction = 1.0f;

    vec3 jumpSpeed = vec3 (0.0f, jmp_spd, 0.0f);
    vec3 gravityAcceleration = vec3 (0.0f, grav_a, 0.0f);
    vec3 acceleration;
    vec3 velocity;
    vec3 position;
    vec3 resultingForce;
    vec3 force_g;

    void Start ();
    void Run (std::ofstream &csv_file);
    void WriteToFile (std::ofstream &csv_file) const;
    void Jump ();
    void SetVelocity (vec3 vec);
    void SetPos (vec3 pos);
    void Move (vec3 step);
    void Update (float deltaT);
    vec3 GetPos () const;
    static vec3 GetPushVector (vec3 enemyPos, vec3 playerPos);
};
