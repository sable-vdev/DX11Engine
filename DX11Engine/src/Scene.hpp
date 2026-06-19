#pragma once
#include <unordered_map>

#include "Camera.hpp"
#include "Entity.hpp"
#include "LightManager.hpp"
#include "Material.hpp"
#include "ObjectLoader.hpp"

class Scene
{
public:
	Scene(ID3D11Device* device, std::string name) :
		m_matrixBuffer(device),
		m_lightBuffer(device),
		m_cameraBuffer(device),
		name(name)
	{
	}

	std::shared_ptr<Model> LoadModel(const std::string& path);
	std::shared_ptr<Material> GetOrCreateMaterial(const std::string& path, ID3D11Device* device);
	Entity* CreateEntity(const std::string& path, std::shared_ptr<Material> material);


	std::vector<std::unique_ptr<Entity>>& GetEntities() { return m_entities; }

	void Update(float dt);
	void Draw(ID3D11DeviceContext* context) const;

private:
	void CalculateWorld(Entity* e, DX::XMMATRIX parent);

public:
	LightManager lightManager;
	std::string name;
	float4 cameraPos = float4(0.0f, 0.f, 0.f, 0.f);
private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
	std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;

	DX11ConstantBuffer<CBDMatrix> m_matrixBuffer;
	DX11ConstantBuffer<LightData> m_lightBuffer;
	DX11ConstantBuffer<CBDCamera> m_cameraBuffer;
};