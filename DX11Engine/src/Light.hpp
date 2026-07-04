#pragma once
#include "EngineTypes.hpp"
#include "Transform.hpp"

struct alignas(16) LightData
{
	//Transform transform;
	float4 position = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 direction = float4(-0.5f, -1.0f, -0.5f, 0.0f);

	float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float specularPower = 32.0f;
	float range = 0.0f;
	float innerCone = 0.0f;
	float outerCone = 0.0f;
	U32 type;
};


struct LightEntity
{
	LightData data;
	//std::optional<LightModel> visual;

	LightEntity(LightData dat) : data(dat)
	{
	};
};
