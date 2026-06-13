#pragma once
#include "EngineTypes.hpp"

struct Transform
{
	float3 position;
	float3 rotation; //EULER FOR NOWWWWWWWWWWWWWWW NEEDS TO BE CHANGED ASAP
	float3 scale;

	Transform* parent = nullptr;

	inline Mat4 GetLocalMatrix() const
	{
		return DX::XMMatrixScaling(scale.x, scale.y, scale.z)
			* DX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
			* DX::XMMatrixTranslation(position.x, position.y, position.z);
	}

	inline Mat4 GetWorldMatrix() const
	{
		if (parent)
			return GetLocalMatrix() * parent->GetWorldMatrix();

		return GetLocalMatrix();
	}

};