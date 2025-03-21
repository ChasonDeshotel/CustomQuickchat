#pragma once
#include <chrono>
#include <functional>
#include <map>

class InputHandler;
class GameState;
class ChatManager;
class KeyBinds;
class ActorWrapper;

class InputHandler {
  public:
    explicit InputHandler(std::function<std::shared_ptr<GameState>()> gameState,
        std::function<std::shared_ptr<ChatManager>()> chatManager);
    void initHooks();

    void init();
    void KeyPress(ActorWrapper caller, void* params, std::string eventName);
    void InitHooks();
    void DestroyHooks();
    void onKeyPress(ActorWrapper caller, void* params, std::string eventName);
    void onKeyRelease(ActorWrapper caller, void* params, std::string eventName);

    void OnKeyPress(int key);
    void SetChatActive(bool isActive);

    void performAction(const Binding& binding);

  private:
    std::function<std::shared_ptr<GameState>()> gameState_;
    std::function<std::shared_ptr<ChatManager>()> chatManager_;
    std::function<std::shared_ptr<KeyBinds>()> keyBinds_;
    bool chatActive_ = false;
    bool hooksActive_ = false;

    bool using_gamepad = false;

    std::map<std::string, bool> keyStates_;
    std::chrono::steady_clock::time_point lastBindingActivated_;

    void updateKeyState(const std::string& keyName, bool pressed);
    void checkBindings(const std::string& keyName);
    bool checkBinding(const Binding& binding, const std::string& keyName);
    void performBindingAction(const Binding& binding);
};