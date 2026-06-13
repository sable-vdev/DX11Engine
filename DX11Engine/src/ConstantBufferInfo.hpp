#pragma once
#include "EngineTypes.hpp"

struct CBDMatrix
{
	Mat4 normal;
	Mat4 model;
	Mat4 mvp;
};

struct CBDSprite
{
	Mat4 mvp;
};

struct CBDCamera
{
	float4 cameraPosition;
};