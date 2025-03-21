#pragma once
#include "InputHandler.h"

#include "SdkHeaders.hpp"
#include <array>
#include <chrono>
#include <string>

#include <TAGame_classes.hpp>
#include <TAGame_parameters.hpp>

class ChatManager {
  public:
    ChatManager();
    //  GetCvar(Cvars::enabled);

    void initHooks();
    void sendMessage(const std::string& chat, EChatChannel chatMode);

    void ProcessChatMessage(const std::string& message);

    auto CanSendMessage() const -> bool; // Checks cooldown and game state

    void sendMessage(const std::string& chat, EChatChannel chatMode, bool log = false);
    void SetChatTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud = nullptr);

    void ResetChatTimeoutMsg();
    void cmd_list_custom_chat_labels(std::vector<std::string> args);
    auto get_last_chat() -> std::string;

    void updateTimeoutMessage(std::string message) { chatTimeoutMsg = message; }

    bool getDisablePostMatch() { return disablePostMatch; };

    // call this when cvars get updated
    void setDisablePostMatch(bool disable) { disablePostMatch = disable; };

  private:
    bool enabled{ true };
    bool disablePostMatch{ true };

    InputHandler& inputHandler_;

    std::string chatTimeoutMsg = "Chat disabled for [Time] second(s).";

    void SetTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud);

    std::chrono::time_point<std::chrono::steady_clock> lastMessageTime_;

    void gfxHudChatPresetCallback(ActorWrapper caller, void* params, std::string eventName);
    void onPressChatPresetCallback(ActorWrapper Caller, void* Params, std::string eventName);
    void notifyChatDisabledCallback(ActorWrapper caller, void* params, std::string eventName);
    void onChatMessageCallback(ActorWrapper caller, void* params, std::string eventName);
    void applyChatSpamFilterCallback(ActorWrapper caller, void* params, std::string eventName);

    void determine_quickchat_labels(UGFxData_Controls_TA* controls = nullptr, bool log = false);
    void apply_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller, UGFxData_Chat_TA_execOnPressChatPreset_Params* params = nullptr);
    void apply_all_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller);

    std::chrono::steady_clock::time_point epochTime;
    std::chrono::steady_clock::time_point lastBindingActivated;

    // modify quickchat UI stuff
    static constexpr std::array<const char*, 4> preset_group_names = { "ChatPreset1", "ChatPreset2", "ChatPreset3", "ChatPreset4" };
    std::array<std::array<FString, 4>, 4> pc_qc_labels;
    std::array<std::array<FString, 4>, 4> gp_qc_labels;

    static constexpr double BLOCK_DEFAULT_QUICKCHAT_WINDOW = 0.1; // maybe turn into a cvar w slider in settings
};