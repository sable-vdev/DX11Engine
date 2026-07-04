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

	bool active = true;
public:
	Entity()
	{
		transform.owner = this;
	}

	~Entity() = default;
	Entity(const Entity& rhs) = default;
	Entity(Entity&& rhs) = default;
	Entity& operator=(const Entity& rhs) = default;
	Entity& operator=(Entity&& rhs) = default;
};