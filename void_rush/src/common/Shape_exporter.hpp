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
    static void export_shape(Shape* shape, std::string name);
private:    

};