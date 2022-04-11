#include "Jump.hpp"

void test::Start ()
{
    std::ofstream csv_file ("../proto_outputs/whatever2.csv");
    csv_file << "time,"
             << "X,"
             << " Y"
             << "\n";

    // Run
    Run (csv_file);
    csv_file.close ();
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
        force_g = gravityAcceleration * MASS;
        // Set resulting force
        resultingForce = force_g;
        // Update acceleration
        acceleration = resultingForce / MASS;
        // Update velocity
        velocity = velocity + acceleration * deltaT;
    }
    else
    {
        if (velocity.x > 0)
        {
            velocity.x -= FRICTION;
        }
        if (velocity.z > 0)
        {
            velocity.z -= FRICTION;
        }
    }
    // Update position
    position = position + velocity * deltaT;
}
void test::Jump ()
{
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
}
void test::Run (std::ofstream &csv_file)
{
    SetVelocity (Vector3 (START_V, START_V, 0.0f));
    while (time < RUN_TIME)
    {
        if (time == JMP_1 || time == JMP_2)
        {
            Jump ();
        }
        Update (deltlaT);
        WriteToFile (csv_file);
        time++;
    }
}