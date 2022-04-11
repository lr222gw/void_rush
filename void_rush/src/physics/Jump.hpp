#pragma once

#include <fstream>
#include <iostream>

#include "../common/Helper.hpp"
#include "../common/Vector.hpp"

#define MASS 50.0f
#define GRAV_A 9.82f
#define JMP_SPD 20.0f
#define START_V 60.0f
#define RUN_TIME 21
#define JMP_1 8
#define JMP_2 13
#define FRICTION 1.0f

struct test
{
    float time = 0;
    float t1, t2;
    float deltlaT = 1;

    bool grounded = false;

    Vector3 jumpSpeed = Vector3 (0.0f, JMP_SPD, 0.0f);
    Vector3 gravityAcceleration = Vector3 (0.0f, GRAV_A, 0.0f);
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
