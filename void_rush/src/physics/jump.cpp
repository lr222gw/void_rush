#include "Jump.hpp"

void test::Start ()
{
    std::ofstream csv_file ("proto_outputs/whatever2.csv");
    if(csv_file.is_open()){
        csv_file << "time,"
             << "X,"
             << " Y"
             << "\n";

        mass = 50.0f;
        grav_a = -9.82f;
        start_v = 60.0f;
        jmp_spd = 20.0f;
        friction = 1.0f;
        time = 0;
        deltlaT = 1;
        gravityAcceleration = Vector3 (0.0f, grav_a, 0.0f);
        jumpSpeed = Vector3 (0.0f, jmp_spd, 0.0f);
        SetVelocity (Vector3 (start_v, start_v, 0.0f));

        // Run
        Run (csv_file);
        csv_file.close ();
    }
   
}
Vector3 test::GetPushVector (Vector3 enemyPos, Vector3 playerPos)
{
    return playerPos - enemyPos;
}
Vector3 test::GetPos () const { return position; }
void test::Move (Vector3 step) { position += step; }
void test::SetPos (Vector3 pos) { position = pos; }
void test::SetVelocity (Vector3 vec) { velocity = vec; }
void test::Update (float deltaT)
{
    if (!grounded)
    {
        // Update forces
        force_g = gravityAcceleration * mass;
        // Set resulting force
        resultingForce = force_g;
        // Update acceleration
        acceleration = resultingForce / mass;
        // Update velocity
        velocity = velocity + acceleration * deltaT;
    }
    else
    {
        if (velocity.x > 0)
        {
            velocity.x -= friction;
        }
        if (velocity.z > 0)
        {
            velocity.z -= friction;
        }
    }
    // Update position
    position = position + velocity * deltaT;
    if(position.y <= 0){
        grounded = true;
        position.y = 0;
    }
}
void test::Jump ()
{
    if(grounded){
        grounded = false;
    }
    if (velocity.y > 0)
    {
        velocity += jumpSpeed;
    }
    else
    {
        velocity = Vector3 (velocity.x, jumpSpeed.y, velocity.z);
    }
}
void test::WriteToFile (std::ofstream &csv_file) const
{
    csv_file << time << "," << position.x << "," << position.y << "\n";
    std::cout<< "Jump.cpp: "<<"X: " << position.x << " Y: " << position.y << "\n";
}
void test::Run (std::ofstream &csv_file)
{

    
    

    while (time < 21)
    {
        if (time == 13 || time == 15)
        {
           Jump ();
        }
        Update (deltlaT);
        WriteToFile (csv_file);
        time++;
    }
}