#include "LightManager.hpp"

void LightManager::AddLight(LightEntity&& light)
{
	m_lights.emplace_back(std::move(light));
}

void LightManager::RemoveLight(I32 id)
{
}
