#pragma once

class ChatManager {
  public:
    ChatManager(InputManager& inputManager);

    void initHooks();

    void ProcessChatMessage(const std::string& message);
    bool CanSendMessage() const; // Checks cooldown and game state

    void SendMessage(const std::string& chat, EChatChannel chatMode, bool log = false);
    void SetChatTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud = nullptr);

  private:
    InputManager& inputManager_;
    std::chrono::time_point<std::chrono::steady_clock> lastMessageTime_;
};