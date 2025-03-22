#include "ObjectProvider.h"
#include <string>

#include "SdkHeaders.hpp"
// #include "GameDefines.hpp"

#include "EngineValidator.h"

ObjectProvider::ObjectProvider(std::function<std::shared_ptr<EngineValidator>()> engineValidator)
  : engineValidator_(std::move(engineValidator)) {}

// ObjectProvider::~ObjectProvider() {
//     m_staticClasses.clear();
//     m_staticFunctions.clear();
//
//     for (UObject* uObject : m_createdObjects) {
//         if (uObject) {
//             MarkForDestroy(uObject);
//         }
//     }
//
//     m_createdObjects.clear();
// }

auto
ObjectProvider::getFullName(UObject* obj) -> std::string {
    if (obj) {
        return obj->GetFullName();
    }
    return "None";
}

auto
ObjectProvider::findStaticClass(const std::string& className) -> class UClass* {
    if (m_staticClasses.empty()) {
        for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i++) {
            if (UObject* uObject = UObject::GObjObjects()->at(i)) {
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
ObjectProvider::findStaticFunction(const std::string& className) -> class UFunction* {
    if (m_staticFunctions.empty()) {
        for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i++) {
            if (UObject* uObject = UObject::GObjObjects()->at(i)) {
                if (uObject->IsA<UFunction>()) {
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

auto
ObjectProvider::getEngine() -> class UEngine* {
    return UEngine::GetEngine();
}

auto
ObjectProvider::getAudioDevice() -> class UAudioDevice* {
    return UEngine::GetAudioDevice();
}

auto
ObjectProvider::getWorldInfo() -> class AWorldInfo* {
    return UEngine::GetCurrentWorldInfo();
}

auto
ObjectProvider::getFileSystem() -> class UFileSystem* {
    return (UFileSystem*)UFileSystem::StaticClass();
}

auto
ObjectProvider::getLocalPlayer() -> class ULocalPlayer* {
    if (UEngine* engine = this->getEngine(); engine && engine->GamePlayers[0]) {
        return engine->GamePlayers[0];
    }
    return nullptr;
}

auto
ObjectProvider::getUniqueID() -> struct FUniqueNetId {
    if (ULocalPlayer* localPlayer = this->getLocalPlayer()) {
        return localPlayer->eventGetUniqueNetId();
    }
    return FUniqueNetId{};
}

auto
ObjectProvider::getHUD() -> AGFxHUD_TA* {
    return this->get<AGFxHUD_TA>();
}

auto
ObjectProvider::getDataStore() -> UGFxDataStore_X* {
    return this->get<UGFxDataStore_X>();
}

auto
ObjectProvider::getOnlinePlayer() -> UOnlinePlayer_X* {
    return this->get<UOnlinePlayer_X>();
}

// fixme
auto
ObjectProvider::checkNotInName(UObject* obj, const std::string& str) -> bool {
    return false;
}

// void
// ObjectProvider::cmd_list_playlist_info(std::vector<std::string> args) {
//     UOnlineGamePlaylists_X* playlists = Instances.getInstanceOf<UOnlineGamePlaylists_X>();
//     if (!playlists)
//         return;
//
//     LOG("DownloadedPlaylists size: {}", playlists->DownloadedPlaylists.size());
//
//     LOG("--------------------------------------");
//     LOG("ID --> Internal name --> Display name");
//     LOG("--------------------------------------");
//     for (const auto& p : playlists->DownloadedPlaylists) {
//         if (!p)
//             continue;
//
//         LOG("{} --> {} --> {}", p->PlaylistId, playlists->IdToName(p->PlaylistId).ToString(), p->getLocalizedName().ToString());
//     }
// }
