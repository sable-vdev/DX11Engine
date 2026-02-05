#include "ObjectLoader.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

#include <chrono>
#include <thread>
#include <future>
#include <memory>


std::unique_ptr<Model> ObjectLoader::LoadObject(std::string filename)
{
    return LoadObjectImple(filename.c_str());
}

std::unique_ptr<Model> ObjectLoader::LoadObject(const char* filename)
{
    return LoadObjectImple(filename);
}

void ObjectLoader::LoadObjectAsync(const char* filename, TSQueue<std::unique_ptr<Model>>& models)
{
    std::thread([filename, &models]() {
        auto model = LoadObjectImple(filename);

        if(model)
            models.Push(std::move(model));
        }).detach();
}

std::unique_ptr<Model> ObjectLoader::LoadObjectImple(const char* filename)
{
    auto startTime = std::chrono::system_clock::now();

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string error = "Assimp error: ";
        error += importer.GetErrorString();

        LOG(error);
        return nullptr;
    }

    std::unique_ptr<Model> model = std::make_unique<Model>(Application::Get().GetDevice());
    std::string name = std::filesystem::path(filename).filename().stem().string();
    model->name = name;

    ProcessNode(*model, scene->mRootNode, scene, std::filesystem::path(filename).parent_path().string());

    if (model->meshes.empty())
    {
        LOG("Failed to create model object, because the file seems to be empty");
        return nullptr;
    }
    
    LOG("Created the model and returning it now!");

    auto endTime = std::chrono::system_clock::now();
    std::string timePassed = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()) + " ms";
    LOG(timePassed);

    return model;
}

void ObjectLoader::ProcessNode(Model& model, aiNode* node, const aiScene* scene, std::string directory)
{
    for (U32 i{}; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        //model.meshes.push_back(
        ProcessMesh(model, mesh, scene, directory);
    }

    for (U32 i{}; i < node->mNumChildren; i++)
    {
        ProcessNode(model, node->mChildren[i], scene, directory);
    }
}

void ObjectLoader::ProcessMesh(Model& model, aiMesh* mesh, const aiScene* scene, std::string& directory)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    std::vector<U32> indices;
    indices.reserve(mesh->mNumFaces * 3);

    for (U32 i{}; i < mesh->mNumVertices; i++)
    {
        Vertex vert{};

        vert.pos.x = mesh->mVertices[i].x;
        vert.pos.y = mesh->mVertices[i].y;
        vert.pos.z = mesh->mVertices[i].z;

        if (mesh->mNormals)
        {
            vert.normal.x = mesh->mNormals[i].x;
            vert.normal.y = mesh->mNormals[i].y;
            vert.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vert.texcoord.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
            vert.texcoord.y = 1.0f - static_cast<float>(mesh->mTextureCoords[0][i].y);
        }

        vertices.push_back(std::move(vert));
    }

    for (U32 i{}; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (U32 j{}; j < face.mNumIndices; j++)
        {
            indices.push_back(std::move(face.mIndices[j]));
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        std::string path = directory + "\\" + str.C_Str();

        model.textures->diffuse = TextureManager::Load(path);
    }

    model.meshes.push_back(Mesh(vertices, indices, Application::Get().GetDevice()));

    //return Mesh(vertices, indices, Application::Get().GetDevice());
}
