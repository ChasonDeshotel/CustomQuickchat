#include "Base.h"
#include "bakkesmod/wrappers/CVarManagerWrapper.h"
#include "imgui_bm/imgui.h"

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

auto
SettingsWindowBase::GetPluginName() -> std::string {
    return "Custom Quickchat";
}

void
SettingsWindowBase::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

auto
PluginWindowBase::GetMenuName() -> std::string {
    return "CustomQuickchat";
}

auto
PluginWindowBase::GetMenuTitle() -> std::string {
    return menuTitle_;
}

void
PluginWindowBase::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

auto
PluginWindowBase::ShouldBlockInput() -> bool {
    return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

auto
PluginWindowBase::IsActiveOverlay() -> bool {
    return true;
}

void
PluginWindowBase::OnOpen() {
    isWindowOpen_ = true;
}

void
PluginWindowBase::OnClose() {
    isWindowOpen_ = false;
}

void
PluginWindowBase::Render() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(800, 600), ImVec2(FLT_MAX, FLT_MAX));
    if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    RenderWindow();

    ImGui::End();

    // Handle window closing after ImGui::End() to avoid ImGui state issues
    if (!isWindowOpen_) {
        // Schedule these commands for the next frame to avoid ImGui state corruption
        // when closing the window during the current frame
        _globalCvarManager->executeCommand("togglemenu " + GetMenuName());
        _globalCvarManager->executeCommand("writeconfig");
    }
}

void
PluginWindowBase::gui_footer_init() {
    fs::path plugin_assets_folder = gameWrapper->GetDataFolder() / "sslow_plugin_assets";
    if (!fs::exists(plugin_assets_folder)) {
        LOG("[ERROR] Folder not found: {}", plugin_assets_folder.string());
        LOG("Will use old ugly settings footer :(");
        return;
    }

    GUI::FooterAssets assets = {
        plugin_assets_folder / "github.png",
        plugin_assets_folder / "discord.png",
        plugin_assets_folder / "youtube.png",
    };

    assets_exist = assets.all_assets_exist();

    if (assets_exist) {
        footer_links = std::make_shared<GUI::FooterLinks>(
            GUI::ImageLink(assets.github_img_path, github_link, github_link_tooltip, footer_img_height),
            GUI::ImageLink(assets.discord_img_path, GUI::discord_link, GUI::discord_desc, footer_img_height),
            GUI::ImageLink(assets.youtube_img_path, GUI::youtube_link, GUI::youtube_desc, footer_img_height));
    } else {
        LOG("One or more plugin asset is missing... will use old ugly settings footer instead :(");
    }
}
