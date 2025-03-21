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
    ChatManager(InputHandler& inputHandler);

    void initHooks();

    void ProcessChatMessage(const std::string& message);
    bool CanSendMessage() const; // Checks cooldown and game state

    void SendMessage(const std::string& chat, EChatChannel chatMode, bool log = false);
    void SetChatTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud = nullptr);

    // chat timeout stuff
    std::string chatTimeoutMsg = "Chat disabled for [Time] second(s).";
    void ResetChatTimeoutMsg();

  private:
    InputHandler& inputHandler_;
    std::chrono::time_point<std::chrono::steady_clock> lastMessageTime_;

    void determine_quickchat_labels(UGFxData_Controls_TA* controls = nullptr, bool log = false);
    void apply_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller, UGFxData_Chat_TA_execOnPressChatPreset_Params* params = nullptr);
    void apply_all_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller);

    std::chrono::steady_clock::time_point epochTime = std::chrono::steady_clock::time_point();
    std::chrono::steady_clock::time_point lastBindingActivated;

    // modify quickchat UI stuff
    static constexpr std::array<const char*, 4> preset_group_names = { "ChatPreset1", "ChatPreset2", "ChatPreset3", "ChatPreset4" };
    std::array<std::array<FString, 4>, 4> pc_qc_labels;
    std::array<std::array<FString, 4>, 4> gp_qc_labels;

    static constexpr double BLOCK_DEFAULT_QUICKCHAT_WINDOW = 0.1; // maybe turn into a cvar w slider in settings
};