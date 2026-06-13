#pragma once
#include <unordered_map>
#include "Entity.hpp"
#include "LightManager.hpp"

class Scene
{
public:
	Scene(ID3D11Device* device) :
		m_matrixBuffer(device),
		m_lightBuffer(device),
		m_cameraBuffer(device)
	{
	}

	std::shared_ptr<Model> LoadModel(const std::string& path)
	{
		auto it = m_models.find(path);

		if (it != m_models.end())
		{
			return it->second;
		}

		std::shared_ptr<Model> model = ObjectLoader::LoadObject(path);
		
		if(model)
			m_models.emplace(path, model);

		return model;
	}

	/*
	std::shared_ptr<ModelTexture> LoadTexture(const std::string& path, ID3D11Device* device)
	{
		auto it = m_materials.find(path);

		if (it != m_materials.end())
			return it->second;

		auto mat = std::make_shared<ModelTexture>()
	}
	*/

	Entity* CreateEntity(const std::string& path)
	{
		Entity* e = &m_entities.emplace_back(std::filesystem::path(path).filename().stem().string());
		e->model = LoadModel(path);
		
	}

	void Draw(ID3D11DeviceContext* context) const
	{
		for (const Entity& e : m_entities)
		{
			if (!e.active || !e.model)
				continue;

			Mat4 world = e.transform.GetWorldMatrix();

			CBDMatrix matrices{};
			matrices.model = world;
			matrices.normal = DX::XMMatrixInverse(nullptr, world);
			matrices.mvp = world * Camera::GetViewMatrix() * Camera::GetProjectionMatrix();

			CBDCamera cam{};
			cam.cameraPosition = cameraPos;


			m_matrixBuffer.BindVS(context, matrices, 0);
			m_cameraBuffer.BindVS(context, cam, 1);
			m_lightBuffer.BindPS(context, lightManager.GetLights()[0].data, 1);

		}
	}

public:
	LightManager lightManager;
	float4 cameraPos;
private:
	std::vector<Entity> m_entities;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;
	std::unordered_map<std::string, std::shared_ptr<ModelTexture>> m_materials;

	DX11ConstantBuffer<CBDMatrix> m_matrixBuffer;
	DX11ConstantBuffer<LightData> m_lightBuffer;
	DX11ConstantBuffer<CBDCamera> m_cameraBuffer;
};