#pragma once
#include "EngineTypes.hpp"

struct Transform
{
	Mat4 local;
	Mat4 world;

	float3 position{ 0.f, 0.f, 0.f };
	float3 rotation{ 0.f, 0.f, 0.f }; //EULER FOR NOWWWWWWWWWWWWWWW NEEDS TO BE CHANGED ASAP
	float3 scale{ 1.f, 1.f, 1.f };

	void UpdateLocal()
	{
		DX::XMMATRIX loc =
			DX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
			DX::XMMatrixTranslation(position.x, position.y, position.z);
		
		DX::XMStoreFloat4x4(&local, loc);
	}
};