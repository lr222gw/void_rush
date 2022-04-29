#include "Shape_exporter.hpp"
#include "Shape.hpp"

Shape_exporter::~Shape_exporter()
{
    
    
}

void Shape_exporter::set_nrOf(int nrOfMeshes, int nrOfMaterials)
{
    this->nrOfMeshes = nrOfMeshes;
    this->nrOfMaterials = nrOfMaterials;
}

void Shape_exporter::init()
{
    this->scene = new aiScene();
    this->scene->mRootNode = new aiNode();
    this->scene->mNumMeshes = this->nrOfMeshes;
    this->scene->mMeshes = new aiMesh * [this->nrOfMeshes]{ nullptr };    
    this->scene->mNumMaterials = this->nrOfMaterials;
    this->scene->mMaterials = new aiMaterial * [this->nrOfMaterials]{ nullptr };
    this->scene->mRootNode->mMeshes = new unsigned int[this->nrOfMeshes];
    this->scene->mRootNode->mNumMeshes = this->nrOfMeshes;

    for (int i = 0; i < nrOfMaterials; i++) {

        scene->mMaterials[i] = new aiMaterial(); //TODO: change this to have more materials!
        aiString texture_test = aiString("textures/outline.png"); //TODO: use a vector with aiStrings for all different materials..
        scene->mMaterials[i]->AddProperty(&texture_test, AI_MATKEY_TEXTURE_DIFFUSE(0));
    }

    //this->scene->mNumTextures = this->nrOfMeshes;
    //this->scene->mTextures = new aiTexture * [this->nrOfMeshes]{nullptr};
    
}

//Shape_exporter* Shape_exporter::get()
//{
//    static Shape_exporter* me = new Shape_exporter();
//    return me;
//}

void Shape_exporter::build_shape_model(Shape* shape, std::string name)
{    
    static int mesh_index = 0; //
    static int material_index = 0; //
    
    
    scene->mMeshes[mesh_index] = new aiMesh();
    scene->mMeshes[mesh_index]->mMaterialIndex = material_index;

    scene->mRootNode->mMeshes[mesh_index] = mesh_index;

    auto pMesh = scene->mMeshes[mesh_index];

    size_t nrOfVertices = shape->planes.size() * 4; //planes have 4 verts each    
    pMesh->mVertices = new aiVector3D[nrOfVertices];
    pMesh->mNumVertices = static_cast<unsigned  int>(nrOfVertices);
    pMesh->mNormals = new aiVector3D[nrOfVertices];

    pMesh->mNumFaces = shape->planes.size();
    pMesh->mFaces = new aiFace[pMesh->mNumFaces];

    /// TEST_
    pMesh->mTextureCoords[0] = new aiVector3D[nrOfVertices];
    pMesh->mNumUVComponents[0] = 2;
    
    //pMesh->mTangents = new aiVector3D[pMesh->mNumVertices];
    //pMesh->mBitangents = new aiVector3D[pMesh->mNumVertices];
    /// TEST^

    unsigned int vert_counter = 0;
    unsigned int face_counter = 0;
    unsigned int vert_per_face_count = 0;
    for (auto plane : shape->planes) {

        aiFace face;
        face.mNumIndices = 4;
        face.mIndices = new unsigned int[4];

        vert_per_face_count = 0;
        //TODO: Why do i get 'Buffer overrun'  warnings here...?
        for (auto point : plane->get_all_points()) {
            vec3 plane_normal = plane->get_normal();

            pMesh->mVertices[vert_counter] = aiVector3D(point->x, point->y, point->z);
            pMesh->mNormals[vert_counter] = aiVector3D(plane_normal.x, plane_normal.y, plane_normal.z);

            /// TESTvvvvvvvvvvvvvv            

            pMesh->mTextureCoords[0][vert_counter] 
                = aiVector3D(
                    plane->uv[vert_per_face_count].x, 
                    plane->uv[vert_per_face_count].y, 0); //TODO: FIX ME

            //pMesh->mBitangents[vert_counter] = aiVector3D(0, 0, 0); //TODO: FIX ME
            //pMesh->mTangents[vert_counter] = aiVector3D(0, 0, 0); //TODO: FIX ME
            /// TEST^^^^^^^^^^
            

            face.mIndices[vert_per_face_count] = vert_counter;
            ++vert_counter;
            ++vert_per_face_count;
        }
        pMesh->mFaces[face_counter++] = face;
        
    }
    

    mesh_index++;
    //material_index++;

    
}

void Shape_exporter::export_final_model(std::string name)
{
    Assimp::Exporter exporter;
    unsigned int flags =  //NOTE: uncertain
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenUVCoords |
        aiProcess_GenNormals;

    //if (exporter.Export(&scene, "obj", "test.obj", flags, properties) != aiReturn_SUCCESS) {
    //if (exporter.Export(&scene, "obj", "assets/obj/"+name + ".obj", aiProcess_FlipUVs) != aiReturn_SUCCESS) {
    if (exporter.Export(scene, "obj", "assets/obj/"+name + ".obj", aiProcess_ConvertToLeftHanded) != aiReturn_SUCCESS) {
        //if (exporter.Export(&scene, "obj", "test.obj") != aiReturn_SUCCESS) {
        std::cout << "Could not save model file" << std::endl;
        std::cout << exporter.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    delete this->scene;
}
