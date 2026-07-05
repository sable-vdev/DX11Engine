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

	return nullptr;
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
	std::unique_ptr<Entity> e = std::make_unique<Entity>();
	std::string name = std::filesystem::path(path).filename().stem().string();
	name.at(0) = std::toupper(name.at(0));
	e->name = name;

	e->model = LoadModel(path);
	if (!e->model)
		return nullptr;

	e->material = std::move(material);

	Entity* ptr = e.get();

	m_entities.push_back(std::move(e));

	return ptr;
}

void Scene::DestroyEntity(Entity* entity)
{
	if (!entity) return;

	std::vector<Entity*> remove;
	remove.push_back(entity);

	for (size_t i{}; i < remove.size(); i++)
	{
		for (Transform* child : remove[i]->transform.children)
		{
			remove.push_back(child->owner);
		}
	}
	
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
		[&](const std::unique_ptr<Entity>& ptr)
		{
			return std::find(remove.begin(), remove.end(), ptr.get()) != remove.end();
		}), 
		m_entities.end());
}

void Scene::Update(float dt)
{
}

void Scene::Draw(ID3D11DeviceContext* context) const
{
	if (!context)
	{
		LOG_ERROR("Context is invalid");
		return;
	}

	for (auto& e : m_entities)
	{
		if (!e->active || !e->model || !e->material)
			continue;

		DX::XMMATRIX worldMat = DX::XMLoadFloat4x4(&e->transform.GetWorldMatrix());

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

Entity* Scene::CreateSkybox(ID3D11Device* device)
{
	if (!device)
		return nullptr;

	std::unique_ptr<Entity> e = std::make_unique<Entity>();
	e->name = "Skybox";

	std::shared_ptr<Model> skyBox = std::make_shared<Model>();
	Mesh mesh{Vertices::Cube::vertices, Vertices::Cube::indices, device};
	
	skyBox->meshes.push_back(std::move(mesh));

	if (!e->model)
		return nullptr;

	e->material = nullptr;

	Entity* ptr = e.get();

	m_entities.push_back(std::move(e));

	return ptr;
}
