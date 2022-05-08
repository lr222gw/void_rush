#include "Shape.hpp"
#include "Shape_exporter.hpp"

Shape::Shape_settings Shape::shape_conf;

Shape::Shape():scale(1,1,1), shapeRadius(0.f) {
    static int index_incrementor = 0;
    this->index = index_incrementor++;
}

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

void Shape::setShape(vec3 center, float distanceToEnd, Shape* prev)
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
        static_assert (true);
        
    }
    
    const int matrixSize = 11; //Should to be Odd...    

    //Center_busy_pair busyMatrix[matrixSize][matrixSize] = { Center_busy_pair() };
    std::vector<std::vector<Center_busy_pair>> busyMatrix { std::vector<Center_busy_pair>() };
    busyMatrix.resize(matrixSize);

    for(int i = 0; i < matrixSize; i++){
        busyMatrix[i].resize(matrixSize);
    }

    static std::vector<Center_Index_Pair> all_previousVoxels;
    static vec3 origo(0.f,0.f,0.f);
    if(center.x == origo.x && center.z == origo.z){ //The offset in y-axis might differ, thus we dont check it.
        all_previousVoxels.clear();
    }

    int first_index = (matrixSize * matrixSize / 2.5f);
    //Sets middle (first ) to true
    busyMatrix[first_index / matrixSize][first_index % matrixSize] = Center_busy_pair{center, true, 0};

    
    int max = shape_conf.maxNrOfVoxels;
    int min = shape_conf.minNrOfVoxels;
    int max_padding = std::clamp((int)(distanceToEnd+1) + shape_conf.max_clamp_padding, 0, (int)(distanceToEnd+1) + shape_conf.max_clamp_padding);
   
    int nrOfVoxels = rand() % (max - min ) + min; // random Number Of Voxels    

    

    nrOfVoxels = std::clamp(nrOfVoxels, min, max_padding);

    int current_index = first_index;    
    int prev_index = first_index;
    vec3 current_center = center;
    vec3 prev_center = center;

    std::stack<int> prev_index_stack;

    prev_index_stack.push((int)this->previousVoxels.size());
    previousVoxels.push_back({ current_center, current_index });

    int extra_iterations_counter = 0;
    bool failed_placement = false;
    int dir = 0;
    int start_dir = 0;
    for (int i = 0; i < nrOfVoxels; i++) {
        //int random_prev_index = rand() % (int)previousVoxels.size();
        
        if(!failed_placement){
            dir = rand() % 4;
        }else{
            dir = (dir + 1) % 4;
        }
        switch (dir) {
        case 0:
            current_index -= 1;
            current_center = current_center + offset_left * 2;
            break;
        case 1:
            current_index += 1;
            current_center = current_center + offset_right * 2;
            break;
        case 2:
            current_index += matrixSize;
            current_center = current_center + offset_back * 2;
            break;
        case 3:
            current_index -= matrixSize;
            current_center = current_center + offset_forward*2;
            break;
        }


        bool collided = false;
        if(prev){
            /*for(int i = 0; i< all_previousVoxels.size(); i++){
                if((all_previousVoxels[i].current_center - current_center).length() < 4.f){
                    collided = true;
                    break;
                }
            }*/
            
            for(int i = 0; i< all_previousVoxels.size(); i++){
                if((all_previousVoxels[i].current_center - current_center).length() < shape_conf.plattform_voxel_margin){
                    collided = true;
                    break;
                }
            }
        }

        if (current_index < matrixSize * matrixSize && 
           current_index >  0 && 
            !busyMatrix[current_index / matrixSize][current_index % matrixSize].isBusy && 
            !collided
            ) 
        {
            
            busyMatrix[current_index / matrixSize][current_index % matrixSize]
                = Center_busy_pair{ current_center ,true, (int)this->previousVoxels.size() };
            prev_index = current_index;
            prev_center = current_center;
            prev_index_stack.push((int)this->previousVoxels.size());
            this->previousVoxels.push_back({ current_center, current_index });
            all_previousVoxels.push_back({ current_center, current_index });
            failed_placement = false;
      
        }
        else 
        {
            if(failed_placement == false)
            {
                start_dir = dir;
                failed_placement = true;
            }
            else if(start_dir == dir)
            {
                int prev_from_stack = -1;
                if(!prev_index_stack.empty()){
                    prev_from_stack = prev_index_stack.top();
                    prev_index_stack.pop();
                }else{
                    nrOfVoxels = i;
                    break;
                }

                current_center = this->previousVoxels[prev_from_stack].current_center;
                current_index = this->previousVoxels[prev_from_stack].current_index;
                prev_index = current_index;
                prev_center = current_center;
                failed_placement = false;
            }

            current_index = prev_index;
            current_center = prev_center;

            i--;                        
        }
        extra_iterations_counter++;
    }

    ////////////////////////////////////////

    bool collided = false;
    if (prev) {

        for (int i = 0; i < all_previousVoxels.size(); i++) {
            if ((all_previousVoxels[i].current_center - previousVoxels.back().current_center).length() < shape_conf.plattform_voxel_margin) {
                collided = true;
                break;
            }
        }
    }

    static int c = 0; 
    if( previousVoxels.size() == 1 && collided){
        //previousVoxels.clear();        
        busyMatrix[first_index / matrixSize][first_index % matrixSize].is_illegal = true;
        previousVoxels.back().is_illegal = true;
        this->is_illegal = true;
        c++;
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
    this->set_InOut_longstDist(nrOfVoxels, center);    
    this->set_InOut_firstLastDeclared(busyMatrix, matrixSize, center);


    

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
    low.y = low.y - this->scale.y * 2;

    vec3_pair min_max{low,high};
     
    bounding_boxes.push_back(min_max);

}

void Shape::buildShape()
{
    for(Center_Index_Pair voxel : this->previousVoxels){
        //if(!voxel.is_illegal){
            this->setShapeCube(voxel.current_center);
        //}
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

void Shape::set_InOut_longstDist(int nrOfVoxels, vec3 &given_center)
{
    LongestDist current;
    LongestDist previous;
    current.endPos = given_center;
    current.startPos = given_center;
    current.distance = 0;    
    vec3* END = nullptr;
    vec3* START = nullptr;
    int actualNrOfVoxels = nrOfVoxels + 1;
    for (int i = 0; i < actualNrOfVoxels - 1; i++) {


        for (int j = i + 1; j < actualNrOfVoxels; j++) {

            float temp = (previousVoxels[i].current_center - previousVoxels[j].current_center).length();

            if (temp > current.distance) {
                current.distance = temp;
                current.startPos = previousVoxels[i].current_center;
                current.endPos = previousVoxels[j].current_center;
            }
        }
    }

    this->inCorner.pos = current.startPos;
    this->outCorner.pos = current.endPos;
    this->shapeRadius = ((current.endPos - current.startPos) / 2).length();
    this->shapeMidpoint = current.startPos + (current.endPos - current.startPos) / 2;
}

void Shape::set_InOut_firstLastDeclared(std::vector<std::vector<Center_busy_pair>> busyMatrix, int matrixsize, vec3& given_center)
{
    bool wasFirst = false;
    vec3* END = nullptr;
    vec3* START = nullptr;
    vec3 last = given_center;
    vec3 first = given_center;
    for (int i = 0; i < matrixsize; i++) {

        for (int j = 0; j < matrixsize; j++) {

            if (busyMatrix[i][j].isBusy) {

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

    /*float off = .2f;
    START->y = START->y + off;
    END->y = END->y - off;*/
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

Offset::Offset(vec3 scale)
{

    left = vec3({ -scale.x,   0,        0 });
    right = vec3({ scale.x,   0,        0 });
    forward= vec3({ 0,       0,  scale.z });
    back = vec3({ 0,          0, -scale.z });
    down = vec3({ 0,   -scale.y,        0 });
    up = vec3({ 0,      scale.y,        0 });
}
