#pragma once
#include <vector>
#include "EngineTypes.hpp"
#include "Light.hpp"

class LightManager
{
public:
	void AddLight(const LightEntity& light);
	void RemoveLight(I32 id);

	const std::vector<LightEntity>& GetLights() const { return m_lights; };
	LightEntity& GetLight(I32 id) { return m_lights.at(id); }
private:
	std::vector<LightEntity> m_lights;
};