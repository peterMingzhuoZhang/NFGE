//====================================================================================================
// Filename:	Debug.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "DebugUI.h"
#include "D3DUtil.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace NFGE::Graphics;

namespace
{
	NFGE::Core::WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return true;
		}
		return false;
	}

	bool IsKeyboardInput(UINT message)
	{
		switch (message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_CHAR:
			return true;
		}
		return false;
	}
}

LRESULT CALLBACK DebugUIMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();

	// When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	if (io.WantCaptureMouse && IsMouseInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	// When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	if (io.WantCaptureKeyboard && IsKeyboardInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	LRESULT result = ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
	if (result != 0)
		return result;

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void DebugUI::StaticInitialize(HWND window, bool docking, bool mutiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto device = Graphics::GetDevice();
	auto context = Graphics::GetContext();

	ImGuiIO& io = ImGui::GetIO();
	if (docking)
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	if (mutiViewport)
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable MutiWindow

	

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);
	
	// Hook application to window's procedure
	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);
}

void DebugUI::StaticTerminate()
{
	// Restore original window's procedure
	sWindowMessageHandler.Unhook();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void NFGE::Graphics::DebugUI::SetTheme(Theme theme)
{
	switch (theme)
	{
	case NFGE::Graphics::DebugUI::Theme::Classic:
		ImGui::StyleColorsClassic();
		break;
	case NFGE::Graphics::DebugUI::Theme::Dark:
		ImGui::StyleColorsDark();
		break;
	case NFGE::Graphics::DebugUI::Theme::Light:
		ImGui::StyleColorsLight();
		break;
	default:
		break;
	}
	
}
