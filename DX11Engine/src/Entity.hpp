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

	virtual ~Entity() = default;
	Entity(const Entity& rhs) = default;
	Entity(Entity&& rhs) = default;
	Entity& operator=(const Entity& rhs) = default;
	Entity& operator=(Entity&& rhs) = default;
};

class Cube : public Entity 
{
public:
	inline static Entity* CreateSkyBoxCube() 
	{

	}

	~Cube() = default;
	Cube(const Cube& rhs) = default;
	Cube(Cube&& rhs) = default;
	Cube& operator=(const Cube& rhs) = default;
	Cube& operator=(Cube&& rhs) = default;
};