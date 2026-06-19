#pragma once
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "../../imgui_lib/imgui.h"

#include "Scene.hpp"

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Begin();
	void Render(Scene& scene);
	void End();

	void OnStart(ImGuiID dockSpaceId, const ImGuiViewport* imGuiViewport);
private:
	void DrawEntity(Entity* entity);
	void DrawHierachy(Scene& scene);
	void DrawInspector();
	void DrawInfo(float frameRate);
	void DrawDirectionalLight(Scene& scene);

	void OnRightClickMenu(Scene& scene);
private:
	Entity* m_selectedEntity = nullptr;
};

