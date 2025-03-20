#include "GameInstances.cpp"

GameInstances::GameInstances() {
    I_UCanvas = nullptr;
    I_AHUD = nullptr;
    I_UGameViewportClient = nullptr;
    I_APlayerController = nullptr;
}

GameInstances::~GameInstances() {
    m_staticClasses.clear();
    m_staticFunctions.clear();

    for (UObject* uObject : m_createdObjects) {
        if (uObject) {
            MarkForDestroy(uObject);
        }
    }

    m_createdObjects.clear();
}

auto
GameInstances::IUEngine() -> class UEngine* {
    return UEngine::GetEngine();
}

auto
GameInstances::IUAudioDevice() -> class UAudioDevice* {
    return UEngine::GetAudioDevice();
}

auto
GameInstances::IAWorldInfo() -> class AWorldInfo* {
    return UEngine::GetCurrentWorldInfo();
}

auto
GameInstances::IUCanvas() -> class UCanvas* {
    return I_UCanvas;
}

auto
GameInstances::IAHUD() -> class AHUD* {
    return I_AHUD;
}

auto
GameInstances::IUFileSystem() -> class UFileSystem* {
    return (UFileSystem*)UFileSystem::StaticClass();
}

auto
GameInstances::IUGameViewportClient() -> class UGameViewportClient* {
    return I_UGameViewportClient;
}

auto
GameInstances::IULocalPlayer() -> class ULocalPlayer* {
    UEngine* engine = IUEngine();

    if (engine && engine->GamePlayers[0]) {
        return engine->GamePlayers[0];
    }

    return nullptr;
}

auto
GameInstances::IAPlayerController() -> class APlayerController* {
    return I_APlayerController;
}

auto
GameInstances::GetUniqueID() -> struct FUniqueNetId {
    ULocalPlayer* localPlayer = IULocalPlayer();

    if (localPlayer) {
        return localPlayer->eventGetUniqueNetId();
    }

    return FUniqueNetId{};
}

auto
GameInstances::GetHUD() -> AGFxHUD_TA* {
    if (hud) {
        return hud;
    }

    return GetInstanceOf<AGFxHUD_TA>();
}

auto
GameInstances::GetDataStore() -> UGFxDataStore_X* {
    if (dataStore) {
        return dataStore;
    }

    return GetInstanceOf<UGFxDataStore_X>();
}

auto
GameInstances::GetOnlinePlayer() -> UOnlinePlayer_X* {
    if (onlinePlayer) {
        return onlinePlayer;
    }

    return GetInstanceOf<UOnlinePlayer_X>();
}
