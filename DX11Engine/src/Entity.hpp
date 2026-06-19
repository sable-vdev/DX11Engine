#pragma once
#include "Transform.hpp"
#include "Model.hpp"
#include "Material.hpp"

class Entity
{
public:
	std::string name;

	Transform transform;
	std::shared_ptr<Model> model;
	std::shared_ptr<Material> material;

	Entity* parent = nullptr;
	std::vector<Entity*> children;

	bool active = true;
public:
	Entity() = default;
	~Entity() = default;

	void RecalculateLocal(Transform& t)
	{
		DX::XMMATRIX local = DX::XMMatrixScaling(t.scale.x, t.scale.y, t.scale.z)
			* DX::XMMatrixRotationRollPitchYaw(t.rotation.x, t.rotation.y, t.rotation.z)
			* DX::XMMatrixTranslation(t.position.x,	t.position.y, t.position.z);
	}
};