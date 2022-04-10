#pragma once

#include <iostream>
#include <fstream>

#include "../common/Vector.hpp"
#include "../common/Helper.h"
#include "../physics/Jump.hpp"

void physics_proto(){
    //Test Start 
    test jumpTest;
    jumpTest.Start();
    //Test End

    float mass = 2.f;
    const double a = -9.81; 
    int t = 0;
  
   
    Vector2 acc;
    Vector2 velocity = {100, 150};
    Vector2 v0 = {100, 150};
    Vector2 gravity;
    Vector2 drag;
    gravity.y = -9.82f;
    

    Vector2 grav = Vector2(0.0f, -9.82f);
    Vector2 v = v0;//Vector2(0.0f, 0.0f);
    Vector2 force_grav = grav*mass;

    Vector2 startPos;
    Vector2 pos;
    float h;
    std::string content;

    std::ofstream csv_file("../proto_outputs/whatever.csv");
    csv_file << "time," <<  "X," <<  " Y" <<  "\n";
    //Jag testar att göra en ändring här
    while (t < 16) {    
        
        // Förlag 1: 
        
        //acc = (force_grav)/mass;

        //v = v+acc*t;

        //pos = pos + v*t;

        //Förslag 2:
        //pos = pos + v*t;

        //acc = (force_grav)/mass;

        //v = v+acc*t;

        //Förslag 3:
        // v.x = v0.x;
        //v.y = gravity.y*t;
        //pos = v*t; 
    
        //Förslag Kaströrelse:
        float angle = 3.14/6;
        pos.x = v0.x*cos(deg_to_rad(30.0f))*t;
        pos.y = (v0.y*sin(deg_to_rad(30.0f)))*t - (9.82*pow(t,2))/2;
        
         std::cout <<  "X:" << pos.x << " Y:" << pos.y << "\n";

        csv_file << t << "," <<  pos.x << "," << pos.y << "\n";
        t++;
    }                    
       
    csv_file.close();
}

