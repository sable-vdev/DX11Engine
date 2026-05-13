#pragma once
#include "EngineTypes.hpp"

struct CBDMatrix
{
	Mat4x4 normal;
	Mat4x4 model;
	Mat4x4 mvp;
};

struct CBDSprite
{
	Mat4x4 mvp;
};
/*
struct CBDLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;

	float specularPower;
	float3 padding0;

	Vec4 lightDirection;
};
*/

struct CBDCamera
{
	float4 cameraPosition;
};