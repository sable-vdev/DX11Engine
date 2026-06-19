#include "Scene.hpp"

std::shared_ptr<Model> Scene::LoadModel(const std::string& path)
{
	auto it = m_models.find(path);

	if (it != m_models.end())
	{
		return it->second;
	}

	std::shared_ptr<Model> model = ObjectLoader::LoadObject(path);

	if (model)
		m_models.emplace(path, model);

	return model;
}

std::shared_ptr<Material> Scene::GetOrCreateMaterial(const std::string& path, ID3D11Device* device)
{
	auto it = m_materials.find(path);

	if (it != m_materials.end())
		return it->second;

	auto mat = std::make_shared<Material>(Material::CreateDefault(device));
	m_materials.emplace(path, mat);

	return mat;
}

Entity* Scene::CreateEntity(const std::string& path, std::shared_ptr<Material> material)
{
	std::unique_ptr<Entity> e = std::make_unique<Entity>(); //m_entities.emplace_back(std::filesystem::path(path).filename().stem().string()).get();
	std::string name = std::filesystem::path(path).filename().stem().string();
	name.at(0) = std::toupper(name.at(0));
	e->name = name;
	e->model = LoadModel(path);
	e->material = std::move(material);

	Entity* ptr = e.get();

	m_entities.push_back(std::move(e));

	return ptr;
}

void Scene::CalculateWorld(Entity* e, DX::XMMATRIX parent)
{
	DX::XMMATRIX local = DX::XMLoadFloat4x4(&e->transform.local);
	DX::XMMATRIX world = local * parent;

	DX::XMStoreFloat4x4(&e->transform.world, world);

	for (Entity* en : e->children)
	{
		CalculateWorld(en, world);
	}
}

void Scene::Update(float dt)
{
	for (auto& e : m_entities)
	{
		if (!e->active) continue;

		e->transform.UpdateLocal();
	}

	for (auto& e : m_entities)
	{
		if (!e->active) continue;

		if (!e->parent)
		{
			CalculateWorld(e.get(), DX::XMMatrixIdentity());
		}
	}
}

void Scene::Draw(ID3D11DeviceContext* context) const
{
	for (auto& e : m_entities)
	{
		if (!e->active || !e->model || !e->material)
			continue;

		DX::XMMATRIX worldMat = DX::XMLoadFloat4x4(&e->transform.world);

		CBDMatrix matrices{};
		DX::XMStoreFloat4x4(&matrices.model, worldMat);
		DX::XMStoreFloat4x4(&matrices.normal, DX::XMMatrixTranspose(DX::XMMatrixInverse(nullptr, worldMat)));
		DX::XMStoreFloat4x4(&matrices.mvp, worldMat * DX::XMLoadFloat4x4(&Camera::GetViewMatrix()) * DX::XMLoadFloat4x4(&Camera::GetProjectionMatrix()));

		CBDCamera cam{};
		cam.cameraPosition = cameraPos;

		e->material->Bind(context, e->model->textures);

		m_matrixBuffer.BindVS(context, matrices, 0);
		m_cameraBuffer.BindVS(context, cam, 1);
		m_lightBuffer.BindPS(context, lightManager.GetLights()[0].data, 2);

		e->model->Draw(context);
	}
}