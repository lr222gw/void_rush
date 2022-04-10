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
    float frictionCheat = 1.0f;

    float mass = 50.0f;
    Vector3 jumpSpeed = Vector3 (0.0f, 20.0f, 0.0f);
    Vector3 gravityAcceleration = Vector3 (0.0f, -9.82f, 0.0f);
    Vector3 acceleration;
    Vector3 velocity;
    Vector3 position;
    Vector3 resultingForce;
    Vector3 force_g;

    void Start ();
    void Run (std::ofstream &csv_file);
    void WriteToFile (std::ofstream &csv_file);
    void Jump ();
    void SetVelocity (Vector3 v);
    void SetPos (Vector3 pos);
    void Move (Vector3 step);
    void Update (float deltaT);
    Vector3 GetPos ();
    Vector3 GetPushVector (Vector3 enemyPos, Vector3 playerPos);
};
