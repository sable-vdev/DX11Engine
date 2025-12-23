#pragma once
#include "EngineTypes.hpp"

struct LightModel
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float specularPower;
	float3 padding0;

	float3 lightDirection;
	float padding1;
};

