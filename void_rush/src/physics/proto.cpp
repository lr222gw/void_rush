#include "proto.hpp"

void physics_proto ()
{
    // Test Start
    test jumpTest;
    jumpTest.Start ();
    // Test End

    float mass = 2.f;
    const double a = -9.81;
    int t = 0;

    Vector2 acc;
    Vector2 velocity = { 100, 150 };
    Vector2 v0 = { 100, 150 };
    Vector2 gravity;
    Vector2 drag;
    gravity.y = -9.82f;
    Vector2 grav = Vector2 (0.0f, -9.82f);
    Vector2 v = v0; // Vector2(0.0f, 0.0f);
    Vector2 force_grav = grav * mass;

    Vector2 startPos;
    Vector2 pos;
    float h; //Not used?
    std::string content;

    std::ofstream csv_file ("../proto_outputs/whatever.csv");
    csv_file << "time,"
             << "X,"
             << " Y"
             << "\n";
    // Jag testar att göra en ändring här
    while (t < 16)
    {


        // Förslag Kaströrelse:
        float angle = 3.14f / 6.0f;
        pos.x = v0.x * cos (deg_to_rad (30.0f)) * (float)t;
        pos.y
            = (v0.y * sinf (deg_to_rad (30.0f))) * (float)t - (9.82f * powf ((float)t, 2.0f)) / 2.0f;

        std::cout << "X:" << pos.x << " Y:" << pos.y << "\n";

        csv_file << t << "," << pos.x << "," << pos.y << "\n";
        t++;
    }

    csv_file.close ();
}
