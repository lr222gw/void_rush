#pragma once

#include <iostream>
#include <fstream>

#include "Vector.hpp"
#include "Helper.h"

struct test{

float time = 0;
float t1, t2;
float deltlaT = 1;

bool grounded = false;
float frictionCheat = 1.0f;

float mass = 50.0f;
Vector3 jumpSpeed = Vector3(0.0f, 20.0f, 0.0f);
Vector3 gravityAcceleration = Vector3(0.0f, -9.82f, 0.0f);
Vector3 acceleration;
Vector3 velocity;
Vector3 position;
Vector3 resultingForce;
Vector3 force_g;

void Start(){
    std::ofstream csv_file("whatever2.csv");
    csv_file << "time," <<  "X," <<  " Y" <<  "\n";

    //Run
    
    Run(csv_file);

    csv_file.close();


}
void Run(std::ofstream& csv_file){
    SetVelocity(Vector3(60.0f, 60.0f, 0.0f));
    while(time < 21){

        if(time == 8 || time == 13){
            Jump();
        }

        Update(deltlaT);
        

        WriteToFile(csv_file);
        time++;
    }

}
void WriteToFile(std::ofstream& csv_file){
    csv_file << time << "," <<  position.x << "," << position.y << "\n";
}
void Jump(){
    if(velocity.y > 0){
        velocity += jumpSpeed;
    }
    else{
        velocity = Vector3(velocity.x, jumpSpeed.y, velocity.z);
    }
}
void SetVelocity(Vector3 v){
    velocity = v;
}
void SetPos(Vector3 pos){
    position = pos;
}
void Move(Vector3 step){
    position += step;
}
void Update(float deltaT){
    if(!grounded){
        //Update forces
        force_g = gravityAcceleration*mass;
        //Set resulting force
        resultingForce = force_g;
        //Update acceleration
        acceleration = resultingForce / mass;
        //Update velocity
        velocity = velocity + acceleration * deltaT;
    }
    else{
        if(velocity.x > 0){
            velocity.x -= frictionCheat;
        }
        if(velocity.z > 0){
            velocity.z -= frictionCheat;
        }
    } 

    //Update position
    position = position + velocity * deltaT;

}
Vector3 GetPos(){
    return position;
}
Vector3 GetPushVector(Vector3 enemyPos, Vector3 playerPos){
    return playerPos-enemyPos;
}
};

