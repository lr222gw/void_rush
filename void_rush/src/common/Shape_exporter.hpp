#pragma once

#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/Exporter.hpp"
#include "assimp/config.h"

#include <string>
#include <iostream> // Output error message

class Shape;
class Shape_exporter{

public:
    Shape_exporter();
    ~Shape_exporter();
    void set_nrOf(int nrOfMeshes, int nrOfMaterials);
    void init();
    void build_shape_model(Shape* shape, std::string name);
    void export_final_model(std::string name);
    aiScene* getScene();
private:
    aiScene* scene;
    int mesh_index;
    int material_index;        

    int nrOfMeshes;
    int nrOfMaterials;
};