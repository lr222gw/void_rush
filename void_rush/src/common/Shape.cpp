#include "Shape.hpp"
#include "Shape_exporter.hpp"

Shape::Shape():scale(1,1,1) {}

Shape::~Shape() {
    for (int i = 0; i < planes.size(); i++) {
        delete planes[i];
    }
    planes.clear();
}

void Shape::addPlane(vec3 a, vec3 b, vec3 c, vec3 d)
{
    planes.push_back(new Plane({ a,b,c,d }));
}

void Shape::setPosition(vec3 pos)
{
    for (int i = 0; i < planes.size(); i++) {
        vec3 offset = vec3::Normalize(planes[i]->get_normal()).mul(scale);
        planes[i]->move(pos + offset);
    }
}

void Shape::move(vec3 pos)
{
    for (int i = 0; i < planes.size(); i++) {
        planes[i]->move(pos);
    }
}

void Shape::setScale(vec3 scale)
{
    this->scale = scale;
}

void Shape::setShape(vec3 center, float distanceToEnd)
{
    setScale(vec3(.5f, .2f, .5f)); // Todo remove!
    vec3 offset_left {      -scale.x, 0,         0       };
    vec3 offset_right {      scale.x, 0,         0       };
    vec3 offset_forward {    0,       0,         scale.z };
    vec3 offset_back {       0,       0,        -scale.z };
    vec3 offset_down {       0,      -scale.y,   0       };
    vec3 offset_up   {       0,      scale.y,    0       };
    
    if(this->planes.size() > 0){
        this->planes.clear();
    }
    
    const int matrixSize = 11; //Should to be Odd...    

    //bool busyMatrix[matrixSize][matrixSize] = { false };
    struct Center_busy_pair {
        vec3 position;
        bool isBusy = false; 
        int index;
    };
    Center_busy_pair busyMatrix[matrixSize][matrixSize] = { Center_busy_pair() };

    int first_index = (matrixSize * matrixSize / 2);
    //int first_index = (matrixSize * matrixSize - matrixSize);

    //Sets middle (first ) to true
    busyMatrix[first_index / matrixSize][first_index % matrixSize] = Center_busy_pair{center, true, 0};
    //setShapeCube(center);

    
    int max = 10;
    int min = 2;
    int mmm = distanceToEnd / max;
    mmm = std::clamp(mmm, min, max);
    //max = distanceToEnd / min;
    //min = distanceToEnd / max;


    int nrOfVoxels = rand() % ((mmm - min <= 0 ? 1 : mmm - min)) + min; // random Number Of Voxels
    //Voxel* root = new Voxel;
    //Voxel* current = root;
    int current_index = first_index;    
    int prev_index = first_index;
    vec3 current_center = center;
    vec3 prev_center = center;

    previousVoxels.push_back({ current_center, current_index });
    int extra_iterations_counter = 0;
    for (int i = 0; i < nrOfVoxels; i++) {
        int random_prev_index = rand() % (int)previousVoxels.size();
        //Voxel* next = new Voxel;
        int dir = rand() % 4;
        switch (dir) {
        case 0:
            //current->leftOf = next;
            current_index -= 1;
            current_center = current_center + offset_left * 2;
            break;
        case 1:
            //current->rightOf = next;
            current_index += 1;
            current_center = current_center + offset_right * 2;
            break;
        case 2:
            //current->behind = next;
            current_index += matrixSize;
            current_center = current_center + offset_back * 2;
            break;
        case 3:
            //current->infront = next;
            current_index -= matrixSize;
            current_center = current_center + offset_forward*2;
            break;
        }

        //std::cout << "prev_index % matrixSize " << prev_index % matrixSize << "\n";
        //std::cout << "current_index % matrixSize " << current_index % matrixSize << "\n";

        if (!busyMatrix[current_index / matrixSize][current_index % matrixSize].isBusy &&
            current_index < matrixSize * matrixSize) 
        {

            busyMatrix[current_index / matrixSize][current_index % matrixSize] = Center_busy_pair{ current_center ,true, (int)this->previousVoxels.size()} ;
            prev_index = current_index;
            prev_center = current_center;
            this->previousVoxels.push_back({current_center, current_index});
            //setShapeCube(current_center);
            
        }
        else 
        {
            i--;
            current_center = this->previousVoxels[random_prev_index].current_center;
            current_index  = this->previousVoxels[random_prev_index].current_index;
        }

        //current = next;
        if(extra_iterations_counter > nrOfVoxels){
            std::cout << "Not Done! " << " \n";
            std::cout << "nrOfVoxels: " << nrOfVoxels << " \n";
            for (int i = 0; i < matrixSize; i++) {

                for (int j = 0; j < matrixSize; j++) {
                    std::cout << busyMatrix[i][j].isBusy << " ";
                }
                std::cout << " \n";

            }
        }
        
        extra_iterations_counter++;

    }

    if (extra_iterations_counter > nrOfVoxels) {
        std::cout << "Extra Iterations: " << extra_iterations_counter - nrOfVoxels << " \n";
    }
    std::cout << "nrOfVoxels: " << nrOfVoxels << " \n";
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            std::cout << busyMatrix[i][j].isBusy << " ";
        }
        std::cout << " \n";

    }

    //Find longest distance
    LongestDist current;
    LongestDist previous;
    current.distance = 0;    
    vec3* END = nullptr;
    vec3* START = nullptr;
    int actualNrOfVoxels = nrOfVoxels + 1;
    for (int i = 0; i < actualNrOfVoxels -1 ;  i++) {
        

        for (int j = i+1; j < actualNrOfVoxels;  j++) {

            float temp = (previousVoxels[i].current_center - previousVoxels[j].current_center).length();

            if(temp > current.distance){
                current.distance = temp;
                current.startPos = previousVoxels[i].current_center;
                current.endPos   = previousVoxels[j].current_center;
            }            
        }
    }

    this->inCorner.pos = current.startPos;
    this->outCorner.pos = current.endPos;


    bool wasFirst = false;
    vec3 last;
    vec3 first;
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            
            if(busyMatrix[i][j].isBusy){

                last = busyMatrix[i][j].position;
                if (!wasFirst) {
                    END = &previousVoxels[busyMatrix[i][j].index].current_center;
                    first = busyMatrix[i][j].position;
                    wasFirst = true;
                }
                START = &previousVoxels[busyMatrix[i][j].index].current_center;
            }
        }
    }
    this->inCorner.pos = last;
    this->outCorner.pos = first;



    this->shapeRadius = ((current.endPos - current.startPos) / 2).length();
    this->shapeMidpoint = current.startPos + ( current.endPos - current.startPos) / 2;


    float off = .2f;
    START->y = START->y + off;
    END->y = END->y - off;

   // vec3 dir = this->inCorner.pos - center;
    //vec3 dir = center - this->inCorner.pos;

    /*this->outCorner.pos = this->outCorner.pos + dir;
    this->inCorner.pos = this->inCorner.pos + dir;
    this->move(dir);*/

    int breakHere = 0;

    //SILLY TEST! 
    /*for (int Y = 0; Y < nrOfVoxels_vert; Y++) {
        for (int X = 0; X < nrOfVoxels_Horz; X++) {            
            for (int Z = 0; Z < nrOfVoxels_Horz; Z++) {
                
                setShapeCube(center
                    + offset_down * (Y * 2)
                    + offset_right * (X * 2)
                    + offset_forward * (Z * 2)
                );
                               
            }
        }
    }*/
    
    //setShapeCube(center + offset_forward  + offset_left );
   /*
    setShapeCube(center + offset_forward  + offset_right);
    setShapeCube(center + offset_forward *3 + offset_left);
    setShapeCube(center + offset_forward *3 + offset_right);
    setShapeCube(center + offset_forward *5 + offset_left);
    setShapeCube(center + offset_forward *5 + offset_right);
    setShapeCube(center + offset_back     + offset_left );
    setShapeCube(center + offset_back     + offset_right);
    setShapeCube(center + offset_back * 3 + offset_left);
    setShapeCube(center + offset_back * 3 + offset_right);
    setShapeCube(center + offset_back * 5 + offset_left);
    setShapeCube(center + offset_back * 5 + offset_right);
   
   
   */
}

void Shape::setShapeCube(vec3 center)
{    
    vec3 vert = center;

    std::vector<Plane*> temp_planes;

    Plane* temp_planes_helper[3]{ new XZ_plane(scale), new XY_plane(scale), new YZ_plane(scale) };

    for (auto plane : temp_planes_helper) {
        
        planes.push_back(plane);
        temp_planes.push_back(planes.back());

        Plane* otherside = new Plane(*plane);
        otherside->swap_windingorder();            
        planes.push_back(otherside);
        temp_planes.push_back(planes.back());
    }
    for (int i = 0; i < temp_planes.size(); i++) {
        temp_planes[i]->offset = vec3::Normalize(temp_planes[i]->get_normal()).mul(scale);
        temp_planes[i]->move(center + temp_planes[i]->offset); //TODO: remove?
    }

    
    auto high = temp_planes[0]->point2;    
    auto low = temp_planes[0]->point4;

    vec3_pair min_max{low,high};
    
    bounding_boxes.push_back(min_max);

}

void Shape::buildShape()
{
    for(Center_Index_Pair voxel : this->previousVoxels){
        this->setShapeCube(voxel.current_center);
    }
}

void Shape::updateBoundingBoxes()
{
    bounding_boxes.clear();
    for(Plane* plane : planes){
        if(plane->normal == vec3(0,1,0)){
            bounding_boxes.push_back(vec3_pair({plane->point4, plane->point2}));
        }
    }
}

void Shape::export_as_obj()
{
    //TODO: this could probably be removed if we wanted...
    //Shape_exporter::export_all(this, "test");

}

Plane::Plane()
{
    uv[0].x = 0;
    uv[0].y = 0;
    uv[1].x = 1;
    uv[1].y = 0;
    uv[2].x = 1;
    uv[2].y = 1;
    uv[3].x = 0;
    uv[3].y = 1;
}

Plane::Plane(vec3 a, vec3 b, vec3 c, vec3 d):
    point1(a), point2(b), point3(c), point4(d)
{
    uv[0].x = 0;
    uv[0].y = 0;
    uv[1].x = 1;
    uv[1].y = 0;
    uv[2].x = 1;
    uv[2].y = 1;
    uv[3].x = 0;
    uv[3].y = 1;
}

//definition if plane functions
void Plane::swap_windingorder() {
    vec3 temp = this->point2;
    this->point2 = this->point4;
    this->point4 = temp;
    update_normal();
}

const vec3 Plane::get_normal() { return normal; }

const vec3 Plane::get_rot() { 

    vec3 ret = this->get_normal(); 
    if(ret.x != 0){
        ret.x = ret.x == 1 ? deg_to_rad(90) : deg_to_rad(180 + 90);
        
    }
    if(ret.y != 0){
        if(ret.y == -1 ){
            ret.x = deg_to_rad(180);
        }
        ret.y = 0;
    }
    
    if(ret.z != 0){
        ret.z = ret.z == 1 ? deg_to_rad(90) : deg_to_rad(180 + 90);
        
    }

    return ret;

}

void Plane::move(const vec3& ofset) {
    this->point1 = this->point1 + ofset;
    this->point2 = this->point2 + ofset;
    this->point3 = this->point3 + ofset;
    this->point4 = this->point4 + ofset;
}

vec3 Plane::get_center()
{
    return (point1+point2+point3+point4)/4;
}

std::vector<vec3*> Plane::get_all_points()
{
    return std::vector<vec3*>{ &point1, & point2, & point3, & point4 };
}

void Plane::update_normal() { 
    normal = vec3((point2 - point1)).X(point4 - point1).Normalize();
}

XZ_plane::XZ_plane(vec3 scale) : 
    Plane()
{//Clockwise windingorder        
    
    point1 = vec3(-1, 0, 1 ).mul(scale) ;   // top left    
    point2 = vec3( 1, 0,  1).mul(scale);    // top right
    point3 = vec3( 1, 0, -1).mul(scale);    // bottom right
    point4 = vec3(-1, 0, -1).mul(scale);    // bottom left
    update_normal();
    
}

XY_plane::XY_plane(vec3 scale) : 
    Plane()
{//Clockwise windingorder
    
    point1 = vec3(-1,  1, 0).mul(scale);    // top left
    point2 = vec3( 1,  1, 0).mul(scale);    // top right
    point3 = vec3( 1, -1, 0).mul(scale);    // bottom right
    point4 = vec3(-1, -1, 0).mul(scale);    // bottom left
    update_normal();
}

YZ_plane::YZ_plane(vec3 scale) : 
    Plane()
{ //Clockwise windingorder
    
    point1 = vec3(0,  1, -1).mul(scale) ;  // top left
    point2 = vec3(0,  1,  1).mul(scale) ;  // top right
    point3 = vec3(0, -1,  1).mul(scale) ;  // bottom right
    point4 = vec3(0, -1, -1).mul(scale) ;  // bottom left
    update_normal();
}