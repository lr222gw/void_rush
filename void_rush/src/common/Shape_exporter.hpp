#pragma once

#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/Exporter.hpp"
#include "assimp/config.h"

#include <string>
#include <iostream>//TEMP; REMOVE LATER

class Shape;
class Shape_exporter{

public:
    ~Shape_exporter();
    void set_nrOf(int nrOfMeshes, int nrOfMaterials);
    void init();
    void build_shape_model(Shape* shape, std::string name);
    void export_final_model(std::string name);
private:
    //static Shape_exporter* get();
    //friend class Shape_exporter;
    aiScene* scene;
    
    int nrOfMeshes;
    int nrOfMaterials;
};