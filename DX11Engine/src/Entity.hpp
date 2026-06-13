#pragma once
#include "Transform.hpp"
#include "Model.hpp"
#include "ObjectLoader.hpp"

class Entity
{
public:
	std::string name;

	Transform transform;
	std::shared_ptr<Model> model;
	std::shared_ptr<ModelTexture> texture;

	bool active = true;

public:
	Entity() = default;

	explicit Entity(const std::string& name) : name(name)
	{
	}
};

