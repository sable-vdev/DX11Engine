#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "EngineTypes.hpp"
#include "Texture.hpp"

class TextureManager
{
public:
	static U32 Load(const std::string& path);
	static Texture* Get(U32 id);

private:
	static std::vector<std::unique_ptr<Texture>> s_textures;
	static std::unordered_map<std::string, U32> s_pathToId;
};

