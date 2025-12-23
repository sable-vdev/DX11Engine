#pragma once
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "../../imgui_lib/imgui.h"

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Render();
};

