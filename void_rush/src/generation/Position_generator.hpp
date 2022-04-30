#pragma once
#include "Platform.hpp"
#include "playerGen.hpp"
#include "3Dproj/Vec.h"
#include <random>


enum class Difficulity {
    easy = 1, medium = 2, hard = 3
};
struct MM{
    Platform* first;
    Platform* last;
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
    Position_generator(int seed, int elements);
    ~Position_generator();
    bool start (Difficulity diff);
    void generate_anchor_positions(int platforms_between_anchors, Difficulity selectedDiff);
    void generate_jumpPoints_positions(Difficulity selectedDiff);
    MM jumpPoint_generation_helper(Platform* start, Platform* end);
    vec3 jumpPoint_create_offset(Platform* plat, vec3& currentMiddle, vec3 start, vec3 end);
    void reset_generation(vec3 player_position);
    void set_seed(int _seed);
    std::vector<Platform*>* getAnchors ();
    std::vector<Platform*>* getJumpPoints ();
    void setNrOfElements(int nrOfElements);
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);

private: // Magic Numbers
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