#include "ObjectProvider.h"

ObjectProvider::GameInstances() {
    I_UCanvas = nullptr;
    I_AHUD = nullptr;
    I_UGameViewportClient = nullptr;
    I_APlayerController = nullptr;
}

ObjectProvider::~GameInstances() {
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
ObjectProvider::FindStaticClass(const std::string& className) -> class UClass* {
    if (m_staticClasses.empty()) {
        for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i++) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject) {
                if ((uObject->GetFullName().find("Class") == 0)) {
                    m_staticClasses[uObject->GetFullName()] = static_cast<UClass*>(uObject);
                }
            }
        }
    }

    if (m_staticClasses.contains(className)) {
        return m_staticClasses[className];
    }

    return nullptr;
}

auto
ObjectProvider::FindStaticFunction(const std::string& className) -> class UFunction* {
    if (m_staticFunctions.empty()) {
        for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i++) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject) {
                if (uObject && uObject->IsA<UFunction>()) {
                    m_staticFunctions[uObject->GetFullName()] = static_cast<UFunction*>(uObject);
                }
            }
        }
    }

    if (m_staticFunctions.contains(className)) {
        return m_staticFunctions[className];
    }

    return nullptr;
}

// Get the default constructor of a class type. Example: UGameData_TA* gameData = GetDefaultInstanceOf<UGameData_TA>();
template<typename T>
T*
ObjectProvider::GetDefaultInstanceOf() {
    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i++) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                if (uObject->GetFullName().find("Default__") != std::string::npos) {
                    return static_cast<T*>(uObject);
                }
            }
        }
    }

    return nullptr;
}

// Get an object instance by it's name and class type. Example: UTexture2D* texture = FindObject<UTexture2D>("WhiteSquare");
template<typename T>
T*
ObjectProvider::FindObject(const std::string& objectName, bool bStrictFind = false) {
    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                std::string objectFullName = uObject->GetFullName();

                if (bStrictFind) {
                    if (objectFullName == objectName) {
                        return static_cast<T*>(uObject);
                    }
                } else if (objectFullName.find(objectName) != std::string::npos) {
                    return static_cast<T*>(uObject);
                }
            }
        }
    }
    return nullptr;
}

// Get the most current/active instance of a class. Example: UEngine* engine = GetInstanceOf<UEngine>();
template<typename T>
T*
ObjectProvider::GetInstanceOf() {
    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                // if (uObject->GetFullName().find("Default__") == std::string::npos)
                if (CheckNotInName(uObject, "Default") && CheckNotInName(uObject, "Archetype") &&
                    CheckNotInName(uObject, "PostGameLobby") && CheckNotInName(uObject, "Test")) {
                    return static_cast<T*>(uObject);
                }
            }
        }
    }

    return nullptr;
}

// Get all active instances of a class type. Example: std::vector<APawn*> pawns = GetAllInstancesOf<APawn>();
template<typename T>
std::vector<T*>
ObjectProvider::GetAllInstancesOf() {
    std::vector<T*> objectInstances;

    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                // if (uObject->GetFullName().find("Default__") == std::string::npos)
                if (CheckNotInName(uObject, "Default") && CheckNotInName(uObject, "Archetype") &&
                    CheckNotInName(uObject, "PostGameLobby") && CheckNotInName(uObject, "Test")) {
                    objectInstances.push_back(static_cast<T*>(uObject));
                }
            }
        }
    }

    return objectInstances;
}

// Get all default instances of a class type.
template<typename T>
std::vector<T*>
ObjectProvider::GetAllDefaultInstancesOf() {
    std::vector<T*> objectInstances;

    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                if (uObject->GetFullName().find("Default__") != std::string::npos) {
                    objectInstances.push_back(static_cast<T*>(uObject));
                }
            }
        }
    }

    return objectInstances;
}

// Get all object instances by it's name and class type. Example: std::vector<UTexture2D*> textures =
// FindAllObjects<UTexture2D>("Noise");
template<typename T>
std::vector<T*>
ObjectProvider::FindAllObjects(const std::string& objectName) {
    std::vector<T*> objectInstances;

    if (std::is_base_of<UObject, T>::value) {
        for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_INTERATE_OFFSET); i > 0; i--) {
            UObject* uObject = UObject::GObjObjects()->at(i);

            if (uObject && uObject->IsA<T>()) {
                if (uObject->GetFullName().find(objectName) != std::string::npos) {
                    objectInstances.push_back(static_cast<T*>(uObject));
                }
            }
        }
    }

    return objectInstances;
}

// ==============================================   CUSTOM    ==============================================

template<typename T>
std::string
ObjectProvider::GetTypeName() {
    std::string objTypeName = typeid(T).name();

    // Remove "class " or "struct " if present
    const std::string classPrefix = "class ";
    const std::string structPrefix = "struct ";

    if (objTypeName.rfind(classPrefix, 0) == 0) {
        objTypeName.erase(0, classPrefix.length()); // Erase the "class " prefix
    } else if (objTypeName.rfind(structPrefix, 0) == 0) {
        objTypeName.erase(0, structPrefix.length()); // Erase the "struct " prefix
    }

    return objTypeName;
}

auto
ObjectProvider::IUEngine() -> class UEngine* {
    return UEngine::GetEngine();
}

auto
ObjectProvider::IUAudioDevice() -> class UAudioDevice* {
    return UEngine::GetAudioDevice();
}

auto
ObjectProvider::IAWorldInfo() -> class AWorldInfo* {
    return UEngine::GetCurrentWorldInfo();
}

auto
ObjectProvider::IUCanvas() -> class UCanvas* {
    return I_UCanvas;
}

auto
ObjectProvider::IAHUD() -> class AHUD* {
    return I_AHUD;
}

auto
ObjectProvider::IUFileSystem() -> class UFileSystem* {
    return (UFileSystem*)UFileSystem::StaticClass();
}

auto
ObjectProvider::IUGameViewportClient() -> class UGameViewportClient* {
    return I_UGameViewportClient;
}

auto
ObjectProvider::IULocalPlayer() -> class ULocalPlayer* {
    UEngine* engine = IUEngine();

    if (engine && engine->GamePlayers[0]) {
        return engine->GamePlayers[0];
    }

    return nullptr;
}

auto
ObjectProvider::IAPlayerController() -> class APlayerController* {
    return I_APlayerController;
}

auto
ObjectProvider::GetUniqueID() -> struct FUniqueNetId {
    ULocalPlayer* localPlayer = IULocalPlayer();

    if (localPlayer) {
        return localPlayer->eventGetUniqueNetId();
    }

    return FUniqueNetId{};
}

auto
ObjectProvider::GetHUD() -> AGFxHUD_TA* {
    if (hud) {
        return hud;
    }

    return GetInstanceOf<AGFxHUD_TA>();
}

auto
ObjectProvider::GetDataStore() -> UGFxDataStore_X* {
    if (dataStore) {
        return dataStore;
    }

    return GetInstanceOf<UGFxDataStore_X>();
}

auto
ObjectProvider::GetOnlinePlayer() -> UOnlinePlayer_X* {
    if (onlinePlayer) {
        return onlinePlayer;
    }

    return GetInstanceOf<UOnlinePlayer_X>();
}

void
ObjectProvider::cmd_list_playlist_info(std::vector<std::string> args) {
    UOnlineGamePlaylists_X* playlists = Instances.GetInstanceOf<UOnlineGamePlaylists_X>();
    if (!playlists)
        return;

    LOG("DownloadedPlaylists size: {}", playlists->DownloadedPlaylists.size());

    LOG("--------------------------------------");
    LOG("ID --> Internal name --> Display name");
    LOG("--------------------------------------");
    for (const auto& p : playlists->DownloadedPlaylists) {
        if (!p)
            continue;

        LOG("{} --> {} --> {}", p->PlaylistId, playlists->IdToName(p->PlaylistId).ToString(), p->GetLocalizedName().ToString());
    }
}
