class MemoryManager {
  public:
    // Set an object's flags to prevent it from being destroyed
    static void MarkInvincible(class UObject* object);

    // Set object as a temporary object and marks it for the garbage collector to destroy
    static void MarkForDestroy(class UObject* object);

    // Memory management for created objects
    static void TrackCreatedObject(class UObject* object);
    static void CleanupCreatedObjects();

  private:
    static std::vector<class UObject*> m_createdObjects;
};