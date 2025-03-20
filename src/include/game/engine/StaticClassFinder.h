class StaticClassFinder {
  public:
    static void Initialize();
    static class UClass* FindStaticClass(const std::string& className);
    static class UFunction* FindStaticFunction(const std::string& functionName);

  private:
    static std::map<std::string, class UClass*> m_staticClasses;
    static std::map<std::string, class UFunction*> m_staticFunctions;
};