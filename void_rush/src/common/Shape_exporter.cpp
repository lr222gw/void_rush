#include "Shape_exporter.hpp"
#include "Shape.hpp"

aiScene::aiScene()
{
    this->mFlags = 0;
    this->mRootNode = NULL;
    this->mNumMeshes = 0;
    this->mMeshes = NULL;
    this->mNumMaterials = 0;
    this->mMaterials = NULL;
    this->mNumAnimations = 0;
    this->mAnimations = NULL;
    this->mNumTextures = 0;
    this->mTextures = NULL;
    this->mNumLights = 0;
    this->mLights = NULL;
    this->mNumCameras = 0;
    this->mCameras = NULL;
    this->mPrivate = NULL;
}

aiScene::~aiScene(){

    for(int i = 0; i < (int)this->mNumMeshes; i++ ){
        delete this->mMeshes[i];

    }
    delete[] this->mMeshes;
    
    for(int i = 0; i < (int)this->mNumMaterials; i++){
        delete this->mMaterials[i];
    }
    delete[] this->mMaterials;

    for (int i = 0; i < (int)this->mRootNode->mNumMeshes; i++) {
        this->mRootNode->mMeshes[i];
    }

    delete[] this->mRootNode->mMeshes;
    free(this->mRootNode);
        
    delete this->mPrivate;

}


Shape_exporter::Shape_exporter() : scene(nullptr)
{
}

Shape_exporter::~Shape_exporter()
{
    
    delete scene;
    
}

void Shape_exporter::set_nrOf(int nrOfMeshes, int nrOfMaterials)
{
    this->nrOfMeshes = nrOfMeshes;
    this->nrOfMaterials = nrOfMaterials;
}

void Shape_exporter::init(texturesEnum texture)
{
    if (scene) {        
        delete scene;    
        scene = nullptr; 
    }
    unsigned int flags 
        = 
         aiProcess_ConvertToLeftHanded
        | aiProcess_GenNormals
        | aiProcess_GenUVCoords
        | aiProcess_CalcTangentSpace
        | aiProcess_JoinIdenticalVertices;    

    std::vector<aiString> textures{ 
        aiString("textures/UnderTexture.png"),
        aiString("textures/outline.png"),
        aiString("textures/Cork.png"),
    };
    //nrOfMaterials = textures.size();

    mesh_index = 0; 
    material_index = 0; 
    this->scene = new aiScene();
    this->scene->mFlags = flags;    

    this->scene->mRootNode = new aiNode();
    this->scene->mNumMeshes = this->nrOfMeshes;
    this->scene->mMeshes = new aiMesh * [this->nrOfMeshes]{ nullptr };    
    this->scene->mNumMaterials = this->nrOfMaterials;
    this->scene->mMaterials = new aiMaterial * [this->nrOfMaterials]{ nullptr };
    this->scene->mRootNode->mMeshes = new unsigned int[this->nrOfMeshes];
    this->scene->mRootNode->mNumMeshes = this->nrOfMeshes;
    

    scene->mMaterials[0] = new aiMaterial(); //TODO: change this to have more materials!        
    //aiString texture_test = textures[0];

    //scene->mMaterials[0]->AddProperty(&texMaterial, AI_MATKEY_TEXTURE_DIFFUSE(0));
    scene->mMaterials[0]->AddProperty(&textures[(int)texture], AI_MATKEY_TEXTURE_DIFFUSE(0));
    aiColor3D diffuse(1.f);
    aiColor3D ambient(0.25f);
    aiColor3D specular(0.25f);
    float shiny = 0.65f;
    scene->mMaterials[0]->AddProperty<aiColor3D>(&diffuse, 1, AI_MATKEY_COLOR_DIFFUSE);
    scene->mMaterials[0]->AddProperty<aiColor3D>(&ambient, 1, AI_MATKEY_COLOR_AMBIENT);
    scene->mMaterials[0]->AddProperty<aiColor3D>(&specular, 1, AI_MATKEY_COLOR_SPECULAR);
    scene->mMaterials[0]->AddProperty<float>(&shiny, 1, AI_MATKEY_SHININESS);
    //c++;

    //for (int i = 0; i < nrOfMaterials; i++) {

        //int c = 0;
        //for(aiString texMaterial : textures){            

        //scene->mMaterials[c] = new aiMaterial(); //TODO: change this to have more materials!        
        //aiString texture_test = textures[c];

        //    scene->mMaterials[c]->AddProperty(&texMaterial, AI_MATKEY_TEXTURE_DIFFUSE(0));
        //    aiColor3D diffuse(1.f);
        //    aiColor3D ambient(0.25f);
        //    aiColor3D specular(0.25f);
        //    float shiny = 0.65f;
        //    scene->mMaterials[c]->AddProperty<aiColor3D>(&diffuse, 1 , AI_MATKEY_COLOR_DIFFUSE);
        //    scene->mMaterials[c]->AddProperty<aiColor3D>(&ambient, 1 , AI_MATKEY_COLOR_AMBIENT);
        //    scene->mMaterials[c]->AddProperty<aiColor3D>(&specular, 1, AI_MATKEY_COLOR_SPECULAR);
        //    scene->mMaterials[c]->AddProperty<float>(&shiny, 1, AI_MATKEY_SHININESS);
        //    c++;
        //}        
    //}
    
}

void Shape_exporter::build_shape_model(Shape* shape, std::string name)
{    
            
    scene->mMeshes[mesh_index] = new aiMesh();
    
    scene->mMeshes[mesh_index]->mMaterialIndex = material_index;

    scene->mRootNode->mMeshes[mesh_index] = mesh_index;

    auto pMesh = scene->mMeshes[mesh_index];
    
    size_t nrOfVertices = shape->planes.size() * 4; //planes have 4 verts each    
    pMesh->mVertices = new aiVector3D[nrOfVertices];
    pMesh->mNumVertices = static_cast<unsigned  int>(nrOfVertices);
    pMesh->mNormals = new aiVector3D[nrOfVertices];

    pMesh->mNumFaces = (int)shape->planes.size();
    pMesh->mFaces = new aiFace[pMesh->mNumFaces];

    pMesh->mTextureCoords[0] = new aiVector3D[nrOfVertices];
    pMesh->mNumUVComponents[0] = 2;
    
    /// TEST_
    pMesh->mTangents = new aiVector3D[pMesh->mNumVertices];
    pMesh->mBitangents = new aiVector3D[pMesh->mNumVertices];
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


            pMesh->mTextureCoords[0][vert_counter] 
                = aiVector3D(
                    plane->uv[vert_per_face_count].x, 
                    plane->uv[vert_per_face_count].y, 0); 

            /// TESTvvvvvvvvvvvvvv            
            pMesh->mBitangents[vert_counter] = aiVector3D(0, 0, 0); //TODO: FIX ME
            pMesh->mTangents[vert_counter] = aiVector3D(0, 0, 0); //TODO: FIX ME
            /// TEST^^^^^^^^^^
            

            face.mIndices[vert_per_face_count] = vert_counter;
            ++vert_counter;
            ++vert_per_face_count;
        }
        pMesh->mFaces[face_counter++] = face;
        
        

    }

    mesh_index++;
}

void Shape_exporter::export_final_model(std::string name)
{
    Assimp::Exporter exporter;
    //unsigned int flags =  //NOTE: uncertain
    //    aiProcess_ConvertToLeftHanded |
    //    aiProcess_JoinIdenticalVertices |
    //    aiProcess_GenUVCoords |
    //    aiProcess_GenNormals;
            
    //if (this->scene && exporter.Export(scene, "obj", "assets/obj/"+name + ".obj", flags) != aiReturn_SUCCESS) {
    if (this->scene && exporter.Export(scene, "obj", "assets/obj/"+name + ".obj", aiProcess_ConvertToLeftHanded) != aiReturn_SUCCESS) {        
        
        std::cout << "Could not save model file" << std::endl;
        std::cout << exporter.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }

}

aiScene* Shape_exporter::getScene()
{
    return this->scene;
}
