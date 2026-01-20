#pragma once

#include <string>
#include <filesystem>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Logger.hpp"
#include "EngineTypes.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

class ObjectLoader
{
public:
	static std::unique_ptr<Model> LoadObject(std::string filename);
	static std::unique_ptr<Model> LoadObject(const char* filename);
private:
	static std::unique_ptr<Model> LoadObjectImple(const char* filename);
	static void ProcessNode(Model& model, aiNode* node, const aiScene* scene);
	static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};