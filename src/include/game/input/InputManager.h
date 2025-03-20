// InputManager should act as the central router for all keypresses
// it should delegate input to the appropriate system based on the current state.
class InputManager {
  public:
    InputManager(ChatManager& chatManager, GameState& gameState);

    void InitHooks();    // Hooks into the game's input function
    void DestroyHooks(); // Unhooks, in case we want to disable input processing

    void OnKeyPress(int key);
    void SetChatActive(bool isActive);

  private:
    ChatManager& chatManager_;
    GameState& gameState_;
    bool chatActive_ = false;
    bool hooksActive_ = false;
};