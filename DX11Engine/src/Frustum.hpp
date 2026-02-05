#pragma once
#include "EngineTypes.hpp"

struct Plane
{
	Vec4 normal;	//unit vector + w component is distance from origin to the nearest point

	float Distance(const Vec4& point) const
	{
		return DX::XMVectorGetX(DX::XMVector4Dot(normal, point));
	}
};

struct Frustum
{
	Plane planes[6];

	void CreateFrustumFromMatrix(const Mat4x4& viewProj);
};