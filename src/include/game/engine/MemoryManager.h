#pragma once

class MemoryManager {
  public:
    MemoryManager();
    ~MemoryManager() = default;

    MemoryManager(const MemoryManager&) = delete;
    auto operator=(const MemoryManager&) -> MemoryManager& = delete;
    MemoryManager(MemoryManager&&) = delete;
    auto operator=(MemoryManager&&) -> MemoryManager& = delete;

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