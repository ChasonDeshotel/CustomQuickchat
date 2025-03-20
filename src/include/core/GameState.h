class GameState {
  public:
    void initHooks();
    bool isGamePaused() const;
    bool isInGame() const;
    bool isMatchEnded() const;

  private:
    bool gamePaused = false;
    bool inGameEvent = false;
    bool matchEnded = false;

    void onPushMenu(ActorWrapper caller, void* params, std::string eventName);
    void onPopMenu(ActorWrapper caller, void* params, std::string eventName);
    void onLoadingScreenStart(std::string eventName);
    void onMatchEnded(ActorWrapper caller, void* params, std::string eventName);
};