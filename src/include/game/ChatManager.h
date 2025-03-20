#pragma once

class ChatManager {
  public:
    ChatManager(InputManager& inputManager);

    void initHooks();

    void ProcessChatMessage(const std::string& message);
    bool CanSendMessage() const; // Checks cooldown and game state

    void SendMessage(const std::string& chat, EChatChannel chatMode, bool log = false);
    void SetChatTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud = nullptr);

    // chat timeout stuff
    std::string chatTimeoutMsg = "Chat disabled for [Time] second(s).";
    void ResetChatTimeoutMsg();

  private:
    InputManager& inputManager_;
    std::chrono::time_point<std::chrono::steady_clock> lastMessageTime_;

    void determine_quickchat_labels(UGFxData_Controls_TA* controls = nullptr, bool log = false);
    void apply_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller, UGFxData_Chat_TA_execOnPressChatPreset_Params* params = nullptr);
    void apply_all_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller);
};