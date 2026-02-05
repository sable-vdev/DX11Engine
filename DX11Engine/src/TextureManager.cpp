#include "TextureManager.hpp"
#include "Application.hpp"


std::unordered_map<std::string, U32> TextureManager::s_pathToId;
std::vector<std::unique_ptr<Texture>> TextureManager::s_textures;;

/*
* Loads a texture if not already cached and returns its id,
* if already loaded returns the existing id
*/
U32 TextureManager::Load(const std::string& path)
{
	if (auto it = s_pathToId.find(path); it != s_pathToId.end())
		return it->second;

	U32 id = static_cast<U32>(s_textures.size());
	s_textures.push_back(std::make_unique<Texture>(Application::Get().GetDevice(), path));

	s_pathToId[path] = id;

	return id;
}

/*
* Retrieves a texture stored in the cache based on an id
*/
Texture* TextureManager::Get(U32 id)
{
	assert(id < s_textures.size());

	return s_textures[id].get();
}
