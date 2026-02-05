#pragma once

#include <string>
#include <filesystem>
#include <mutex>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Logger.hpp"
#include "EngineTypes.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "containers/TSQueue.hpp"

class ObjectLoader
{
public:
	static std::unique_ptr<Model> LoadObject(std::string filename);
	static std::unique_ptr<Model> LoadObject(const char* filename);
	static void LoadObjectAsync(const char* filename, TSQueue<std::unique_ptr<Model>>& m_models);
private:
	static std::unique_ptr<Model> LoadObjectImple(const char* filename);
	static void ProcessNode(Model& model, aiNode* node, const aiScene* scene, std::string directory);
	static void ProcessMesh(Model& model, aiMesh* mesh, const aiScene* scene, std::string& directory);
	//static void LoadTexture()
};