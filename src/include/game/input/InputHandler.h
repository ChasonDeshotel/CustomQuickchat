#pragma once

class InputHandler {
  public:
    InputHandler(
        ChatManager& chatManager, GameState& gameState),
      std::shared_ptr<KeyBindSettings> settings,
               std::shared_ptr<ChatModule> chatModule);

    void InitHooks();
    void DestroyHooks();
    void onKeyPress(ActorWrapper caller, void* params, std::string eventName);
    void onKeyRelease(ActorWrapper caller, void* params, std::string eventName);

    void OnKeyPress(int key);
    void SetChatActive(bool isActive);

    void performAction(const Binding& binding);

  private:
    ChatManager& chatManager_;
    GameState& gameState_;
    bool chatActive_ = false;
    bool hooksActive_ = false;

    bool using_gamepad = false;

    std::shared_ptr<KeyBindSettings> settings_;
    std::shared_ptr<ChatModule> chatModule_;
    std::map<std::string, bool> keyStates_;
    std::chrono::steady_clock::time_point lastBindingActivated_;

    void updateKeyState(const std::string& keyName, bool pressed);
    void checkBindings(const std::string& keyName);
    bool checkBinding(const Binding& binding, const std::string& keyName);
    void performBindingAction(const Binding& binding);
};