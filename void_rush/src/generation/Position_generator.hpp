#pragma once
#include "Platform.hpp"
#include "Player_jump_checker.hpp"
#include "3Dproj/Vec.h"
#include <random>


enum class Difficulity {
    easy = 1, medium = 2, hard = 3
};
struct FirstLast_between_Anchor{
    Platform* first = nullptr;
    Platform* last = nullptr;
};
class Position_generator
{
private:
    int seed;
    int elements;
    Platform*startPlat;
    std::vector<Platform*> anchors;
    std::vector<Platform*> jumpPoints;
    Player_jump_checker* pl;
    friend class ImguiManager;
public:
    Platform* firstJumpPoint;

    Position_generator(int seed);
    ~Position_generator();
    bool start (Difficulity diff);
    void generate_anchor_positions(Difficulity selectedDiff);
    void generate_jumpPoints_positions(Difficulity selectedDiff);        

    FirstLast_between_Anchor jumpPoint_generation_helper(Platform* start, Platform* end);    
    void jumpPoint_create_offset(Platform* plat, vec3& currentMiddle, vec3 start, vec3 end);
    void reset_generation(vec3 player_position);
    void set_seed(int _seed);
    std::vector<Platform*>* getAnchors ();
    std::vector<Platform*>* getJumpPoints ();
    void setNrOfElements(int nrOfElements);
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);

    Platform* getFirstJumppoint();
    Platform* getFirstAnchorpoint();
    int getNrOfValidJumppoints();
    int getNrOfValidAnchorpoints();

    Platform*& GetStartPlatform();

    void removeOverlappingPlatformVoxels();

private: // Magic Numbers
    struct Anchor_point_settings{        
        float minStepMod = 2.f; //2 is half, recommended
        float stepMax = 50.f;    //Max distance between platforms
        float stepMaxHeight = 10.f;    //Max distance between platforms
        float stepMinHeight = -10.f;    //Max distance between platforms
        float lowest_Height = -100.f;   //Lowest point for generation
        float minZAngle = 0.f;
        float spawn_Y_offset_origo = -10.f;
    };
    Anchor_point_settings AP_conf;

    struct Jump_point_settings{        
        float random_dist_dividier = 2.f; //2 is half, recommended...
        float y_min_clamp = -0.2f;    //between -1 and 1
        float y_max_clamp = 0.1f;     //between -1 and 1
        float rand_dir_min_angle_percent = -1.f;    
        float rand_dir_max_angle_percent = 1.f;

    };
    Jump_point_settings JP_conf;

private:  //Imgui stuff
    struct Imgui_data {
        bool useOrigo = true;
    };
    Imgui_data imgui_conf;

};