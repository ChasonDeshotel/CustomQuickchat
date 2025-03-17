#include "Base.h"
#include "imgui_bm/imgui.h"
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

std::string SettingsWindowBase::GetPluginName()
{
	return "Custom Quickchat";
}

void SettingsWindowBase::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string PluginWindowBase::GetMenuName()
{
	return "CustomQuickchat";
}

std::string PluginWindowBase::GetMenuTitle()
{
	return menuTitle_;
}

void PluginWindowBase::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

bool PluginWindowBase::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

bool PluginWindowBase::IsActiveOverlay()
{
	return true;
}

void PluginWindowBase::OnOpen()
{
	isWindowOpen_ = true;
}

void PluginWindowBase::OnClose()
{
	isWindowOpen_ = false;
}

void PluginWindowBase::Render()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(800, 600), ImVec2(FLT_MAX, FLT_MAX));
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	RenderWindow();

	ImGui::End();

	// Handle window closing after ImGui::End() to avoid ImGui state issues
	if (!isWindowOpen_)
	{
		// Schedule these commands for the next frame to avoid ImGui state corruption
		// when closing the window during the current frame
		_globalCvarManager->executeCommand("togglemenu " + GetMenuName());
		_globalCvarManager->executeCommand("writeconfig");
	}
}
