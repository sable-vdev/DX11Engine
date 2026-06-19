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
private:
	Entity* m_selectedEntity = nullptr;
};

