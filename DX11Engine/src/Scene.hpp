#pragma once

#include "LightManager.hpp"

struct Scene
{
	LightManager lightManager;
	float4 cameraPos;

	Scene() = default;
};

