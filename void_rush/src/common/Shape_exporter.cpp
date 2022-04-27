#include "Shape_exporter.hpp"
#include "Shape.hpp"

Shape_exporter::~Shape_exporter()
{
}

void Shape_exporter::export_shape(Shape* shape, std::string name) {
    aiScene scene;

    scene.mRootNode = new aiNode();

    scene.mMeshes = new aiMesh * [1]{ nullptr };
    //scene.mMeshes[0] = nullptr;
    scene.mNumMeshes = 1;

    scene.mMaterials = new aiMaterial * [1]{ nullptr };
    //scene.mMaterials[0] = nullptr;
    scene.mNumMaterials = 1;
    scene.mMaterials[0] = new aiMaterial();

    scene.mMeshes[0] = new aiMesh();
    scene.mMeshes[0]->mMaterialIndex = 0;

    scene.mRootNode->mMeshes = new unsigned int[1];
    scene.mRootNode->mMeshes[0] = 0;
    scene.mRootNode->mNumMeshes = 1;

    auto pMesh = scene.mMeshes[0];

    size_t nrOfVertices = shape->planes.size() * 4; //planes have 4 verts each
    //size_t numValidPoints = nrOfVertices;
    pMesh->mVertices = new aiVector3D[nrOfVertices];
    pMesh->mNumVertices = static_cast<unsigned  int>(nrOfVertices);
    pMesh->mNormals = new aiVector3D[nrOfVertices];

    pMesh->mNumFaces = shape->planes.size();
    pMesh->mFaces = new aiFace[pMesh->mNumFaces];

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
            face.mIndices[vert_per_face_count] = vert_counter;
            ++vert_counter;
            ++vert_per_face_count;
        }

        pMesh->mFaces[face_counter++] = face;

    }

    Assimp::Exporter exporter;
    //Assimp::ExportProperties* properties = new Assimp::ExportProperties;
    //properties->SetPropertyBool(AI_CONFIG_EXPORT_POINT_CLOUDS, true);
    unsigned int flags =  //NOTE: uncertain
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenUVCoords |
        aiProcess_GenNormals;

    //if (exporter.Export(&scene, "obj", "test.obj", flags, properties) != aiReturn_SUCCESS) {
    if (exporter.Export(&scene, "obj", name + ".obj", aiProcess_FlipUVs) != aiReturn_SUCCESS) {
        //if (exporter.Export(&scene, "obj", "test.obj") != aiReturn_SUCCESS) {
        std::cout << "Could not save model file" << std::endl;
        std::cout << exporter.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
}
