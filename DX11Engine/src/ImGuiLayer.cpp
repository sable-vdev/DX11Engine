#include "ImGuiLayer.hpp"
#include "Application.hpp"

ImGuiLayer::ImGuiLayer()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

void ImGuiLayer::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	static bool show_demo_window = true;

	if (show_demo_window)
		ImGui::ShowDemoWindow();

	ImGui::Begin("Info");
	ImGui::Checkbox("Demo Window", &show_demo_window);
	ImGui::SameLine();
	if(ImGui::Checkbox("Vsync", &Application::Get().GetVsync()))
		Application::Get().ToggleVsync(Application::Get().GetVsync());

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	io.DisplaySize = ImVec2(static_cast<float>(Application::Get().GetWidth()),
		static_cast<float>(Application::Get().GetHeight()));

	ImGui::Render();

	//auto deviceContext = Engine::Get().GetD3DContext();
	//auto renderTarget = Engine::Get().GetRenderTargetView();
	//deviceContext->OMSetRenderTargets(1, &renderTarget, nullptr);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
