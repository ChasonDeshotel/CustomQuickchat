class Input {
  public:
    void initHooks();
    void registerBinding(const Binding& binding);
    void performBindingAction(const Binding& binding);

  private:
    std::map<std::string, bool> keyStates;
    std::chrono::steady_clock::time_point lastBindingActivated;
    std::vector<Binding> bindings;
    bool usingGamepad = false;

    void onKeyPressed(ActorWrapper caller, void* params, std::string eventName);
    void resetAllFirstButtonStates();
};