#include "ObjectLoader.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"

#include <chrono>
#include <memory>


std::unique_ptr<Model> ObjectLoader::LoadObject(std::string filename)
{
    return LoadObjectImple(filename.c_str());
}

std::unique_ptr<Model> ObjectLoader::LoadObject(const char* filename)
{
    return LoadObjectImple(filename);
}

std::unique_ptr<Model> ObjectLoader::LoadObjectImple(const char* filename)
{
    auto startTime = std::chrono::system_clock::now();

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::string error = std::format("Assimp error: {}", importer.GetErrorString());

        LOG_ERROR(error);
        return nullptr;
    }

    std::unique_ptr<Model> model = std::make_unique<Model>();
    std::string name = std::filesystem::path(filename).filename().stem().string();
    model->name = name;

    ProcessNode(*model, scene->mRootNode, scene, std::filesystem::path(filename).parent_path().string());

    if (model->meshes.empty())
    {
        LOG_ERROR("Failed to create model object, because the file seems to be empty");
        return nullptr;
    }
    
    LOG_INFO("Created the model and returning it now!");

    auto endTime = std::chrono::system_clock::now();
    auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    LOG_DEBUG("Loaded '" + model->name + "' in " + std::to_string(timePassed) + " ms");;

    return model;
}

void ObjectLoader::ProcessNode(Model& model, aiNode* node, const aiScene* scene, std::string directory)
{
    for (U32 i{}; i < node->mNumMeshes; i++)
    {
        ProcessMesh(model, scene->mMeshes[node->mMeshes[i]], scene, directory);
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

        vert.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        if (mesh->mNormals)
        {
            vert.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
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
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        std::string path = directory + "\\" + str.C_Str();

        model.textures.diffuse = TextureManager::Load(path);
    }

    model.meshes.push_back(Mesh(vertices, indices, Application::Get().GetDevice()));
}
