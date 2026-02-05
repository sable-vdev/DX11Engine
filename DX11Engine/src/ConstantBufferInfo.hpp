#pragma once
#include "EngineTypes.hpp"

struct alignas(16) CBDMatrix
{
	Mat4x4 normal;
	Mat4x4 model;
	Mat4x4 mvp;
};

struct alignas(16) CBDSprite
{
	Mat4x4 mvp;
};

struct alignas(16) CBDLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float specularPower;
	float3 padding0;

	Vec4 lightDirection;
};

struct alignas(16) CBDCamera
{
	float4 cameraPosition;
};