#pragma once
#include "3Dproj/Vec.h"
#include "Helper.hpp"
#include <vector>
#include <stack>

class Shape_exporter;
struct inCorner {
    vec3 pos;
    std::vector<vec3> points;
};

struct outCorner {
    vec3 pos;
    std::vector<vec3> points;
};

struct line
{
    vec3 point1;
    vec3 point2;
};

struct Plane
{   
    Plane();    
    Plane(const Plane* ref);
    Plane(vec3 a, vec3 b, vec3 c, vec3 d);
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 point4;
    vec3 normal;
    vec3 offset;
    vec2 uv[4];
    
    void swap_windingorder();
    const vec3 get_normal();;
    const vec3 get_rot();;
    void move(const vec3& ofset);
    vec3 get_center();
    std::vector<vec3*> get_all_points();    
protected:
    void update_normal();;
};

struct Voxel_matrix {
    vec3*** voxels;
};


struct XZ_plane : public Plane
{
    XZ_plane(vec3 scale);    
    XZ_plane(const Plane* ref) : Plane(ref) {  };
};

struct XY_plane : public Plane
{
    XY_plane(vec3 scale);
    XY_plane(const Plane* ref) :Plane(ref) { };

};
struct YZ_plane : public Plane
{ 
    YZ_plane(vec3 scale);
    YZ_plane(const Plane* ref) :Plane(ref) {  };
};

struct Normals{
     vec3 up =  vec3(0, 1, 0);
     vec3 down = vec3(0, -1, 0);
     vec3 right = vec3(0, 0, 1);
     vec3 left = vec3(0, 0, -1);
     vec3 front = vec3(1, 0, 0);
     vec3 back = vec3(-1, 0, 0);
};


struct LongestDist {
    vec3 startPos;
    vec3 endPos;
    float distance = 0;
};

struct vec3_pair {
    vec3 first;
    vec3 second;
};

struct Center_Index_Pair {
    vec3 current_center;
    int current_index;
    bool is_illegal = false;
};

struct Offset{
    Offset(vec3 scale);
    vec3 left;
    vec3 right;
    vec3 forward;
    vec3 back;
    vec3 down;
    vec3 up;
};

struct Center_busy_pair {
    vec3 position;
    bool isBusy = false;
    int index = -1;
    bool is_illegal = false;
};

class Shape
{
public:
    Shape();
    ~Shape();
    void addPlane(vec3 a, vec3 b, vec3 c, vec3 d);
    void setPosition(vec3 pos);    
    void move(vec3 pos);
    void setScale(vec3 scale);

    void setAnchorShape(vec3 center, float distanceToEnd, Shape* prev = nullptr);
    void setJumppointShape(vec3 center, float distanceToEnd, Shape* prev = nullptr);
    void setShapeCube(vec3 center);
    void set_is_Illegal(bool status);

    void remove_all_planes();

    void buildShape();

    bool get_is_Illegal();
    vec3 get_midpoint();
    vec3 get_scale();

    void set_InOut_longstDist(int nrOfVoxels, vec3& given_center);
    void set_InOut_firstLastDeclared(std::vector<std::vector<Center_busy_pair>> busyMatrix, int matrixsize, vec3& given_center);
    void update_InOut(Shape* prev = nullptr);

    void export_as_obj();
    inCorner inCorner;
    outCorner outCorner;

    Shape* top = nullptr;
    Shape* sides = nullptr;
    Shape* bottom = nullptr;

    std::vector<Center_Index_Pair> previousVoxels;    

    struct Shape_settings{
        //NOTE: ! Some values might be overridden in Game Constructor!
        vec3 default_scale = vec3(0.5f,0.2f,0.5f);
        int maxNrOfVoxels_JP = 25;
        int minNrOfVoxels_JP = 1;
        int maxNrOfVoxels_AP = 6;
        int minNrOfVoxels_AP = 4;
        int max_clamp_padding = 0;
        float plattform_voxel_margin = 2.f;
        bool tryRandom = false;
        float scaleAnchor_XY_Factor = 3;
        int randomOccurances = 2; // use random everytime the random index is even nmbr
        int matrixSize = 11; // use random everytime the random index is even nmbr
    };
    static struct Shape_settings shape_conf; //same for all instances...

    //std::vector<DirectX::XMFLOAT4*> bounding_boxes; //TODO: Handle memory here!
    //std::vector<vec3[2]> bounding_boxes; //TODO: Handle memory here!
    std::vector<vec3_pair> bounding_boxes; //TODO: Handle memory here!

private: 
    friend class Shape_exporter;
    friend class ImguiManager;

    void init_shape_bottomTopSides();
    void updateBoundingBoxes();
    void setShape(vec3 center, int nrOfVoxels, Shape* prev = nullptr);
    void setInOutPoints(vec3 center, Shape* prev = nullptr);

    

    int index = 0;
    static int index_incrementor;

    std::vector<Plane*> planes;
    static Normals normals;
    vec3 scale;
    vec3 shapeMidpoint;
    float shapeRadius;

    bool is_illegal = false;
    
    vec3 offset_left   ;
    vec3 offset_right  ;
    vec3 offset_forward;
    vec3 offset_back   ;
    vec3 offset_down   ;
    vec3 offset_up     ;

};
