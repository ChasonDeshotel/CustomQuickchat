#pragma once
#include "Utils.h"
#include "pch.h"

static constexpr int32_t INSTANCES_INTERATE_OFFSET = 100;

class InstancesComponent {
  public:
    InstancesComponent();
    ~InstancesComponent();

    // Get the most current/active instance of a class, if one isn't found it creates a new instance. Example: UEngine* engine =
    // GetInstanceOf<UEngine>();
    template<typename T>
    T* GetOrCreateInstance() {
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

            return CreateInstance<T>();
        }

        return nullptr;
    }

    // Creates a new transient instance of a class which then adds it to globals.
    // YOU are required to make sure these objects eventually get eaten up by the garbage collector in some shape or form.
    // Example: UObject* newObject = CreateInstance<UObject>();
    template<typename T>
    T* CreateInstance() {
        T* returnObject = nullptr;

        if (std::is_base_of<UObject, T>::value) {
            T* defaultObject = GetDefaultInstanceOf<T>();
            UClass* staticClass = T::StaticClass();

            if (defaultObject && staticClass) {
                returnObject = static_cast<T*>(defaultObject->DuplicateObject(defaultObject, defaultObject->Outer, staticClass));
            }

            // Making sure newly created object doesn't get randomly destoyed by the garbage collector when we don't want it do.
            if (returnObject) {
                MarkInvincible(returnObject);
                m_createdObjects.push_back(returnObject);
            }
        }

        return returnObject;
    }

  private:
    class UCanvas* I_UCanvas;
    class AHUD* I_AHUD;
    class UGameViewportClient* I_UGameViewportClient;
    class APlayerController* I_APlayerController;

  public
    : // Use these functions to access these specific class instances, they will be set automatically; always remember to null check!
    class UEngine* IUEngine();
    class UAudioDevice* IUAudioDevice();
    class AWorldInfo* IAWorldInfo();
    class UCanvas* IUCanvas();
    class AHUD* IAHUD();
    class UGameViewportClient* IUGameViewportClient();
    class ULocalPlayer* IULocalPlayer();
    class APlayerController* IAPlayerController();
    class UFileSystem* IUFileSystem();
    struct FUniqueNetId GetUniqueID();

  public:
    AGFxHUD_TA* hud = nullptr;
    UGFxDataStore_X* dataStore = nullptr;
    USaveData_TA* saveData = nullptr;
    UOnlinePlayer_X* onlinePlayer = nullptr;
    UClass* notificationClass = nullptr;

    AGFxHUD_TA* GetHUD();
    UGFxDataStore_X* GetDataStore();
    UOnlinePlayer_X* GetOnlinePlayer();
};

extern class InstancesComponent Instances;