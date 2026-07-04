#pragma once
#include "EngineTypes.hpp"
#include "Logger.hpp"

class Entity;

class Transform
{
public:
	void SetPosition(const float3& position)
	{
		localPosition = position;

		MarkDirty();
	}

	void SetRotation(const float3& rotation)
	{
		localRotation = rotation;

		MarkDirty();
	}

	void SetScale(const float3& scale)
	{
		localScale = scale;

		MarkDirty();
	}

	const Mat4& GetWorldMatrix()
	{
		if (dirty)
		{
			DX::XMMATRIX loc =
				DX::XMMatrixScaling(localScale.x, localScale.y, localScale.z) *
				DX::XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z) *
				DX::XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);

			DX::XMStoreFloat4x4(&local, loc);

			if (parent)
			{
				DX::XMStoreFloat4x4(&world, loc * (DX::XMLoadFloat4x4(&parent->GetWorldMatrix())));
			}
			else world = local;

			dirty = false;
		}

		return world;
	}

	void AddChild(Transform* child)
	{
		children.push_back(child);
		child->parent = this;
		child->MarkDirty();
	}

	bool SetParent(Transform* parentCandidate)
	{
		if (parentCandidate == this)
			return false;

		for (Transform* t = parentCandidate; t; t = t->parent)
		{
			if (t == this)
				return false;
		}

		if (parent)
		{
			auto& siblings = parent->children;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		}

		parent = parentCandidate;

		if (parent)
		{
			parent->AddChild(this);
			LOG_INFO("Pushed back new child to parent");
		}

		return true;
	}

	Transform() = default;
	~Transform()
	{
		if (parent)
		{
			auto& siblings = parent->children;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		}

		for (Transform* child : children)
		{
			child->parent = nullptr;
			child->MarkDirty();
		}

		parent = nullptr;
	}
	Transform(const Transform& rhs) = default;
	Transform(Transform&& rhs) = default;
	Transform& operator=(Transform&& rhs) = default;
	Transform& operator=(const Transform& rhs) = default;

private:
	void MarkDirty()
	{
		if (dirty) 
			return;
		
		dirty = true;

		for (Transform* child : children)
			child->MarkDirty();
	}

public:
	Mat4 local = {};
	Mat4 world = {};

	Entity* owner = nullptr;
	Transform* parent = nullptr;
	std::vector<Transform*> children;

	float3 localPosition{ 0.f, 0.f, 0.f };
	float3 localRotation{ 0.f, 0.f, 0.f }; //EULER FOR NOWWWWWWWWWWWWWWW NEEDS TO BE CHANGED ASAP
	float3 localScale{ 1.f, 1.f, 1.f };

	bool dirty = true;
};