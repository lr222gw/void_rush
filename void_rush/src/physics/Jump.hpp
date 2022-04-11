#pragma once

#include <fstream>
#include <iostream>

#include "../common/Helper.hpp"
#include "../common/Vector.hpp"


struct test
{
    float time = 0;
    float t1, t2;
    float deltlaT = 1;

    bool grounded = false;

    float mass = 50.0f;
    float grav_a = -9.82;
    float start_v = 60.0f;
    float jmp_spd = 20.0f;
    float friction = 1.0f;

    Vector3 jumpSpeed = Vector3 (0.0f, jmp_spd, 0.0f);
    Vector3 gravityAcceleration = Vector3 (0.0f, grav_a, 0.0f);
    Vector3 acceleration;
    Vector3 velocity;
    Vector3 position;
    Vector3 resultingForce;
    Vector3 force_g;

    void Start ();
    void Run (std::ofstream &csv_file);
    void WriteToFile (std::ofstream &csv_file) const;
    void Jump ();
    void SetVelocity (Vector3 vec);
    void SetPos (Vector3 pos);
    void Move (Vector3 step);
    void Update (float deltaT);
    Vector3 GetPos () const;
    static Vector3 GetPushVector (Vector3 enemyPos, Vector3 playerPos);
};
