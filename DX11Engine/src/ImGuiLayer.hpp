#pragma once
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "../../imgui_lib/imgui.h"

#include "Model.hpp"

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Begin();

	void ModelInfo(Model& model);
	void Render();
	void End();
};

