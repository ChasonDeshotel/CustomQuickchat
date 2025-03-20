class Config {
  public:
    void initCvars();
    CVarWrapper getCvar(const CvarData& cvar);
    CVarWrapper registerCvar_Bool(const CvarData& cvar, bool startingValue);
    // Other cvar methods...

  private:
    // Cvar storage or references
};
