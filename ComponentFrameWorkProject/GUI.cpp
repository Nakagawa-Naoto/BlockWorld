//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"


///////////////////////////////////////////////////////////////////////////////
// GUI class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void GUI::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* devContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& m_io = ImGui::GetIO();
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, devContext);
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void GUI::Uninit(void)
{

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

//-----------------------------------------------------------------------------
// �����_�����O�O����
//-----------------------------------------------------------------------------
void GUI::Begin(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

//-----------------------------------------------------------------------------
// �����_�����O�㏈��
//-----------------------------------------------------------------------------
void GUI::End(void)
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
