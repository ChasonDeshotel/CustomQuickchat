#pragma once

class ChatManager {
  public:
    ChatManager(InputManager& inputManager);

    void ProcessChatMessage(const std::string& message);
    bool CanSendMessage() const; // Checks cooldown and game state
    void SendMessage(const std::string& message);

  private:
    InputManager& inputManager_;
    std::chrono::time_point<std::chrono::steady_clock> lastMessageTime_;
};