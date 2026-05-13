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

void ImGuiLayer::ModelInfo(Model& model)
{
	ImGui::Begin(model.name.c_str());
	ImGui::DragFloat3("Position", &model.GetPosition().x);
	ImGui::DragFloat3("Rotation", &model.GetRotation().x);
	ImGui::DragFloat3("Scale", &model.GetScale().x);
	ImGui::End();
}

void ImGuiLayer::Render()
{
	ImGuiIO& io = ImGui::GetIO();

	//Dockspace stuff
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* imGuiViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(imGuiViewport->WorkPos);
	ImGui::SetNextWindowSize(imGuiViewport->WorkSize);
	ImGui::SetNextWindowViewport(imGuiViewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("Dockspace", nullptr, windowFlags);
	ImGui::PopStyleVar(2);

	ImGuiID dockspaceId = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	static bool firstRun = true;

	if (firstRun)
	{
		firstRun = false;

		ImGui::DockBuilderRemoveNode(dockspaceId);
		ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceId, imGuiViewport->WorkSize);

		ImGuiID dockMainId = dockspaceId;

		ImGuiID dockLeft, dockRight;
		ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Left, 0.2f, &dockLeft, &dockMainId);
		ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.2f, &dockRight, &dockMainId);

		ImGui::DockBuilderDockWindow("Viewport", dockMainId);
		//ImGui::DockBuilderDockWindow("Directional Light", dockMainId);
		//ImGui::DockBuilderDockWindow("Info", dockMainId);

		ImGui::DockBuilderFinish(dockspaceId);
	}

	static bool show_demo_window = false;
	if (show_demo_window)
		ImGui::ShowDemoWindow();

	//windows

	ImGui::Begin("Info");
	ImGui::Checkbox("Demo Window", &show_demo_window);
	ImGui::SameLine();
	if(ImGui::Checkbox("Vsync", &Application::Get().GetVsync()))
		Application::Get().ToggleVsync(Application::Get().GetVsync());

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	ImGui::Begin("Viewport");
	ImVec2 size = ImGui::GetContentRegionAvail();

	ImGui::Image((ImTextureID)Application::Get().GetSRV(), size);

	ImGui::End();

	ImGui::Begin("Directional Light");
	auto light = Application::Get().GetScene().lightManager.GetLight(0);

	ImGui::DragFloat3("Position", &light.data.position.x);
	ImGui::DragFloat3("Rotation", &light.data.direction.x);
	ImGui::DragFloat4("Ambient", &light.data.ambient.x);
	ImGui::DragFloat4("Diffuse", &light.data.diffuse.x);
	ImGui::DragFloat4("Specular", &light.data.specular.x);
	ImGui::End();

	ImGui::End();

	io.DisplaySize = ImVec2(static_cast<float>(Application::Get().GetWidth()),
		static_cast<float>(Application::Get().GetHeight()));
}

void ImGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}