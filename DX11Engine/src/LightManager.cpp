#include "LightManager.hpp"

void LightManager::AddLight(const LightEntity& light)
{
	m_lights.push_back(light);
}

void LightManager::RemoveLight(I32 id)
{
}
