#pragma once
#include "Platform.hpp"
#include "Player_jump_checker.hpp"
#include "3Dproj/Vec.h"
#include <random>

struct mapDimensions {
    float x_width;
    float z_width;
    vec2 lowPoint;
    vec2 highPoint;
};

enum class Difficulity {
    easy = 1, medium = 2, hard = 3
};
struct FirstLast_between_Anchor{
    Platform* first = nullptr;
    Platform* last = nullptr;
};

struct powerUp_positions {
    int nrOfPositions;
    std::vector<vec3> positions;
};
struct Enemy_positions {
    int nrOfPositions;
    struct PositionGroup {
        std::vector<vec3> ranmdomPositions;
        std::vector<vec3> outsidePositions;
        std::vector<vec3> patternPositions;
    };
    std::vector<PositionGroup> positions;        
};

struct ShortCut_positions {
    int nrOfPositions;
    std::vector<vec3> positions;
};

class Position_generator
{
private:
    friend class ImguiManager;
    int seed;    
    Platform*startPlat;
    std::vector<Platform*> anchors;
    std::vector<Platform*> jumpPoints;
    Player_jump_checker* pl;
    powerUp_positions powerup_positions;
    Enemy_positions enemy_positions;
    ShortCut_positions shortcut_positions;
    
    std::vector<Platform*> getAllPlatforms();
    std::vector<Platform*> getInOrderVector_ValidJumppoints();
    std::vector<Platform*> getInOrderVector_ValidAnchors();
public:
    Platform* firstJumpPoint;

    Position_generator(int seed);
    ~Position_generator();
    bool start (Difficulity diff);
    void generate_anchor_positions(Difficulity selectedDiff);
    void generate_jumpPoints_positions(Difficulity selectedDiff);   

    void generate_shortcut();
    void select_powerUp_positions();
    void select_enemy_positions();
    powerUp_positions* get_powerUp_positions();
    Enemy_positions* get_enemy_positions();
    ShortCut_positions* get_shortcut_positions();

    FirstLast_between_Anchor jumpPoint_generation_helper(Platform* start, Platform* end);    
    void jumpPoint_create_offset(Platform* plat, vec3& currentMiddle, vec3 start, vec3 end);
    void reset_generation(vec3 player_position);
    void set_seed(int _seed);
    std::vector<Platform*>* getAnchors ();
    std::vector<Platform*>* getJumpPoints ();
    void setNrOfAnchors(int nrOfElements);
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);

    Platform* getFirstJumppoint();
    Platform* getFirstAnchorpoint();
    int getNrOfValidJumppoints();
    int getNrOfValidAnchorpoints();    
    mapDimensions getCurrentMapDimensions();

    Platform*& GetStartPlatform();

    void removeOverlappingPlatformVoxels();
    void removeUnnecessaryPlatformsVoxels();

private: // Magic Numbers
    struct Anchor_point_settings{        
        int nrOfAnchors = 6; 
        float minStepMod = 2.f; //2 is half, recommended
        float stepMax = 100.f;    //Max distance between platforms
        float stepMaxHeight = 0.f;    //Max distance between platforms
        float stepMinHeight = -10.f;    //Max distance between platforms
        float lowest_Height = -300.f;   //Lowest point for generation
        float freeFallModifier = -50.f;
        int freeFallRate = 3; // Procentage chance would be 1 / freeFallRate
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
        float minimumShortcutDotAngle = 0.75f;

    };
    Jump_point_settings JP_conf;

    static struct PowerUp_position_settings{        
        int powerUp_occurance_rate = 5;
        vec3 position_offset = vec3(0.f, 1.f,0.f);
        
    }PU_conf;

    static struct enemy_Position_settings {
        vec3 enemy_offset = vec3(0.f, 0.f, 0.f);
        float outsideOffset = 5.f;
    }enemyPos_conf;

private:  //Imgui stuff
    struct Imgui_data {
        bool useOrigo = true;
    };
    Imgui_data imgui_conf;

};