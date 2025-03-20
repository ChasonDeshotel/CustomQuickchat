#pragma once

class GameInstances {
  public:
    GameInstances();
    ~GameInstances();

    // Core game instance getters
    class UEngine* GetEngine();
    class UAudioDevice* GetAudioDevice();
    class AWorldInfo* GetWorldInfo();
    class UCanvas* GetCanvas();
    class AHUD* GetHUD();
    class UGameViewportClient* GetGameViewportClient();
    class ULocalPlayer* GetLocalPlayer();
    class APlayerController* GetPlayerController();
    class UFileSystem* GetFileSystem();
    struct FUniqueNetId GetUniqueID();

    // Game-specific instance getters
    AGFxHUD_TA* GetGFxHUD();
    UGFxDataStore_X* GetDataStore();
    UOnlinePlayer_X* GetOnlinePlayer();

    // Template methods for finding/creating instances
    template<typename T>
    T* GetDefaultInstanceOf();
    template<typename T>
    T* GetInstanceOf();
    template<typename T>
    T* GetOrCreateInstance();
    template<typename T>
    std::vector<T*> GetAllInstancesOf();
    template<typename T>
    std::vector<T*> GetAllDefaultInstancesOf();
    template<typename T>
    T* FindObject(const std::string& objectName, bool bStrictFind = false);
    template<typename T>
    std::vector<T*> FindAllObjects(const std::string& objectName);
    template<typename T>
    T* CreateInstance();

  private:
    bool CheckNotInName(UObject* obj, const std::string& str);
    template<typename T>
    std::string GetTypeName();

    // Cached instances
    class UCanvas* I_UCanvas;
    class AHUD* I_AHUD;
    class UGameViewportClient* I_UGameViewportClient;
    class APlayerController* I_APlayerController;

    // Game-specific cached instances
    AGFxHUD_TA* hud;
    UGFxDataStore_X* dataStore;
    USaveData_TA* saveData;
    UOnlinePlayer_X* onlinePlayer;
};