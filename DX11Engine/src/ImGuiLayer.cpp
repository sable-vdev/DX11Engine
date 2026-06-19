#include "ImGuiLayer.hpp"
#include "Application.hpp"
#include <imgui_internal.h>

ImGuiLayer::ImGuiLayer()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.IniFilename = "imgui.ini";
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0;
	style.ChildBorderSize = 0;
	style.PopupBorderSize = 0;
	style.FrameBorderSize = 0;

	style.Colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);

	ImGui_ImplWin32_Init(Application::Get().GetHWND());
	ImGui_ImplDX11_Init(Application::Get().GetDevice(), Application::Get().GetContext());
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyPlatformWindows();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::Render(Scene& scene)
{
	ImGuiIO& io = ImGui::GetIO();

	//Dockspace stuff
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	const ImGuiViewport* imGuiViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(imGuiViewport->WorkPos);
	ImGui::SetNextWindowSize(imGuiViewport->WorkSize);
	ImGui::SetNextWindowViewport(imGuiViewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Dockspace", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceId = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	static bool firstRun = true;
	if (firstRun)
	{
		OnStart(dockspaceId, imGuiViewport);
		firstRun = false;
	}

	//windows

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Viewport");
	ImVec2 size = ImGui::GetContentRegionAvail();

	ImGui::Image((ImTextureID)Application::Get().GetSRV(), size);

	ImGui::End();
	ImGui::PopStyleVar();

	DrawInfo(io.Framerate);
	DrawInspector();
	DrawDirectionalLight(scene);
	DrawHierachy(scene);

	ImGui::End();

	io.DisplaySize = ImVec2(static_cast<float>(Application::Get().GetWidth()),
		static_cast<float>(Application::Get().GetHeight()));
}

void ImGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::DrawEntity(Entity* entity)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (m_selectedEntity == entity)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool opened = ImGui::TreeNodeEx(entity, flags, "%s", entity->name.c_str());

	if (ImGui::IsItemClicked())
		m_selectedEntity = entity;

	if (opened)
	{
		for (Entity* entity : entity->children)
		{
			DrawEntity(entity);
		}

		ImGui::TreePop();
	}
}

void ImGuiLayer::DrawHierachy(Scene& scene)
{
	if (ImGui::Begin("Hierachy"))
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode(scene.name.c_str()))
		{
			for (auto& e : scene.GetEntities())
			{
				if (!e->parent)
				{
					DrawEntity(e.get());
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("ContextMenu");
		}

		OnRightClickMenu(scene);
	}
	ImGui::End();
}

void ImGuiLayer::DrawInspector()
{
	ImGui::Begin("Inspector");
	if (m_selectedEntity)
	{
		ImGui::Separator();

		ImGui::DragFloat3(
			"Position",
			&m_selectedEntity->transform.position.x,
			0.1f);

		ImGui::DragFloat3(
			"Rotation",
			&m_selectedEntity->transform.rotation.x,
			0.1f);

		ImGui::DragFloat3(
			"Scale",
			&m_selectedEntity->transform.scale.x,
			0.1f);
	}
	ImGui::End();
}

void ImGuiLayer::DrawInfo(float frameRate)
{
	static bool show_demo_window = false;
	if (show_demo_window)
		ImGui::ShowDemoWindow();

	ImGui::Begin("Info");
	ImGui::Checkbox("Demo Window", &show_demo_window);
	ImGui::SameLine();

	static bool vsync = Application::Get().GetVsync();
	if (ImGui::Checkbox("Vsync", &vsync))
		Application::Get().ToggleVsync(vsync);

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / frameRate, frameRate);
	ImGui::End();
}

void ImGuiLayer::DrawDirectionalLight(Scene& scene)
{
	ImGui::Begin("Directional Light");
	auto& light = scene.lightManager.GetLight(0);

	ImGui::DragFloat3("Position", &light.data.position.x);
	ImGui::DragFloat3("Rotation", &light.data.direction.x);
	ImGui::DragFloat4("Ambient", &light.data.ambient.x);
	ImGui::DragFloat4("Diffuse", &light.data.diffuse.x);
	ImGui::DragFloat4("Specular", &light.data.specular.x);
	ImGui::End();
}

void ImGuiLayer::OnStart(ImGuiID dockSpaceId, const ImGuiViewport* imGuiViewport)
{
	ImGui::DockBuilderRemoveNode(dockSpaceId);
	ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockSpaceId, imGuiViewport->WorkSize);
	
	ImGuiID dockMainId = dockSpaceId;
	
	ImGuiID dockLeft, dockRight, dockRightBottom;
	ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Left, 0.2f, &dockLeft, &dockMainId);
	ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.2f, &dockRight, &dockMainId);
	ImGui::DockBuilderSplitNode(dockRight, ImGuiDir_Down, 0.2f, &dockRightBottom, &dockRight);
	
	ImGui::DockBuilderDockWindow("Viewport", dockMainId);
	ImGui::DockBuilderDockWindow("Directional Light", dockRight);
	ImGui::DockBuilderDockWindow("Info", dockRightBottom);
	ImGui::DockBuilderDockWindow("Inspector", dockRight);
	ImGui::DockBuilderDockWindow("Hierachy", dockLeft);
	
	ImGui::DockBuilderFinish(dockSpaceId);
}

void ImGuiLayer::OnRightClickMenu(Scene& scene)
{
	if (ImGui::BeginPopupContextItem("ContextMenu"))
	{
		if (ImGui::BeginMenu("Open"))
		{
			if (ImGui::BeginMenu("Entity"))
			{
				if (ImGui::MenuItem("New"))
				{
					std::string path = "";
					if (Window::OpenWin32FileDialog(path) && !path.empty())
					{
						auto defMaterial = scene.GetOrCreateMaterial("default", Application::Get().GetDevice());

						scene.CreateEntity(path, defMaterial);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}