#pragma once

class GameState {
  public:
    void initHooks();
    [[nodiscard]] auto isGamePaused() const -> bool { return gamePaused; }
    [[nodiscard]] auto isInGame() const -> bool { return inGameEvent; };
    [[nodiscard]] auto isMatchEnded() const -> bool { return matchEnded; };

  private:
    bool gamePaused = false;
    bool inGameEvent = false;
    bool matchEnded = false;

    bool onLoadComplete = false;

    void onPushMenu(ActorWrapper caller, void* params, std::string eventName);
    void onPopMenu(ActorWrapper caller, void* params, std::string eventName);
    void onLoadingScreenStart(std::string eventName);
    void onMatchEnded(ActorWrapper caller, void* params, std::string eventName);
};