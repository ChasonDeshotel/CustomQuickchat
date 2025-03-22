#pragma once
#include <algorithm>
#include <functional>
#include <string>

#include <SdkHeaders.hpp>

#include <typeindex>

class UOnlinePlayer_X;
class UGFxDataStore_X;
static constexpr int32_t INSTANCES_ITERATE_OFFSET = 100;

class EngineValidator;

class ObjectProvider {
  public:
    explicit ObjectProvider(std::function<std::shared_ptr<EngineValidator>()> engineValidator);

    ObjectProvider(const ObjectProvider&) = delete;
    auto operator=(const ObjectProvider&) -> ObjectProvider& = delete;
    ObjectProvider(ObjectProvider&&) = delete;
    auto operator=(ObjectProvider&&) -> ObjectProvider& = delete;

    static void Initialize();
    static void Cleanup();

    // Generic getter with caching
    template<typename T>
    auto get() -> T* {
        // Use type_index as the key (more efficient than type_info)
        std::type_index typeIdx(typeid(T));

        // Check if we already have an instance in the cache
        auto it = instanceCache_.find(typeIdx);
        if (it != instanceCache_.end()) {
            return static_cast<T*>(it->second);
        }

        // Not in cache, get the instance
        T* instance = getInstanceOf<T>();

        // Store in cache if found
        if (instance) {
            instanceCache_[typeIdx] = instance;
        }

        return instance;
    }

    template<typename T>
    auto getInstanceOf() -> T* {
        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
                UObject* uObject = UObject::GObjObjects()->at(i);

                if (uObject && uObject->IsA<T>()) {
                    // if (uObject->getFullName().find("Default__") == std::string::npos)
                    if (this->checkNotInName(uObject, "Default") && this->checkNotInName(uObject, "Archetype") &&
                        this->checkNotInName(uObject, "PostGameLobby") && this->checkNotInName(uObject, "Test")) {
                        return static_cast<T*>(uObject);
                    }
                }
            }
        }

        return nullptr;
    }

    // get all active instances of a class type. Example: std::vector<APawn*> pawns = GetAllInstancesOf<APawn>();
    template<typename T>
    auto getAllInstancesOf() -> std::vector<T*> {
        std::vector<T*> objectInstances;

        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
                UObject* uObject = UObject::GObjObjects()->at(i);

                if (uObject && uObject->IsA<T>()) {
                    // if (uObject->getFullName().find("Default__") == std::string::npos)
                    if (this->checkNotInName(uObject, "Default") && this->checkNotInName(uObject, "Archetype") &&
                        this->checkNotInName(uObject, "PostGameLobby") && this->checkNotInName(uObject, "Test")) {
                        objectInstances.push_back(static_cast<T*>(uObject));
                    }
                }
            }
        }

        return objectInstances;
    }

    // get all default instances of a class type.
    template<typename T>
    auto getAllDefaultInstancesOf() -> std::vector<T*> {
        std::vector<T*> objectInstances;

        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
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

    // get all object instances by its name and class type. Example: std::vector<UTexture2D*> textures =
    // FindAllObjects<UTexture2D>("Noise");
    template<typename T>
    auto findAllObjects(const std::string& objectName) -> std::vector<T*> {
        std::vector<T*> objectInstances;

        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
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

    template<typename T>
    auto getTypeName() -> std::string {
        std::string objTypeName = typeid(T).name();

        // Remove "class " or "struct " if present
        const std::string classPrefix = "class ";
        const std::string structPrefix = "struct ";

        if (objTypeName.starts_with(classPrefix)) {
            objTypeName.erase(0, classPrefix.length()); // Erase the "class " prefix
        } else if (objTypeName.starts_with(structPrefix)) {
            objTypeName.erase(0, structPrefix.length()); // Erase the "struct " prefix
        }

        return objTypeName;
    }

    // get the default constructor of a class type. Example: UGameData_TA* gameData = GetDefaultInstanceOf<UGameData_TA>();
    template<typename T>
    auto getDefaultInstanceOf() -> T* {
        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = 0; i < (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i++) {
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

    // get an object instance by its name and class type. Example: UTexture2D* texture = FindObject<UTexture2D>("WhiteSquare");
    template<typename T>
    auto findObject(const std::string& objectName, bool bStrictFind) -> T* {
        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
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

    // Creates a new transient instance of a class which then adds it to globals.
    // YOU are required to make sure these objects eventually get eaten up by the garbage collector in some shape or form.
    // Example: UObject* newObject = CreateInstance<UObject>();
    template<typename T>
    auto createInstance() -> T* {
        T* returnObject = nullptr;

        if (std::is_base_of_v<UObject, T>) {
            T* defaultObject = this->getDefaultInstanceOf<T>();
            UClass* staticClass = T::StaticClass();

            if (defaultObject && staticClass) {
                returnObject = static_cast<T*>(defaultObject->DuplicateObject(defaultObject, defaultObject->Outer, staticClass));
            }

            // Making sure newly created object doesn't get randomly destroyed by the garbage collector when we don't want it do.
            if (returnObject) {
                MarkInvincible(returnObject);
                createdObjects_.push_back(returnObject);
            }
        }

        return returnObject;
    }

    // Get the most current/active instance of a class, if one isn't found it creates a new instance. Example: UEngine* engine =
    // GetInstanceOf<UEngine>();
    template<typename T>
    auto getOrCreateInstance() -> T* {
        if (std::is_base_of_v<UObject, T>) {
            for (int32_t i = (UObject::GObjObjects()->size() - INSTANCES_ITERATE_OFFSET); i > 0; i--) {
                UObject* uObject = UObject::GObjObjects()->at(i);

                if (uObject && uObject->IsA<T>()) {
                    // if (uObject->GetFullName().find("Default__") == std::string::npos)
                    if (this->checkNotInName(uObject, "Default") && this->checkNotInName(uObject, "Archetype") &&
                        this->checkNotInName(uObject, "PostGameLobby") && this->checkNotInName(uObject, "Test")) {
                        return static_cast<T*>(uObject);
                    }
                }
            }

            return this->createInstance<T>();
        }

        return nullptr;
    }

    auto getFullName(UObject* obj) -> std::string;
    auto findStaticClass(const std::string& className) -> class UClass*;
    auto findStaticFunction(const std::string& functionName) -> class UFunction*;

    // Core game instance getters
    auto getEngine() -> class UEngine*;
    auto getAudioDevice() -> class UAudioDevice*;
    auto getWorldInfo() -> class AWorldInfo*;
    auto getHUD() -> class AGFxHUD_TA*;
    auto getLocalPlayer() -> class ULocalPlayer*;
    auto getPlayerController() -> class APlayerController*;
    auto getFileSystem() -> class UFileSystem*;
    struct FUniqueNetId getUniqueID();

    auto getDataStore() -> UGFxDataStore_X*;
    auto getOnlinePlayer() -> UOnlinePlayer_X*;

    auto checkNotInName(UObject* obj, const std::string& str) -> bool;

  private:
    std::function<std::shared_ptr<EngineValidator>()> engineValidator_;

    std::unordered_map<std::type_index, void*> instanceCache_;

    std::map<std::string, class UClass*> m_staticClasses;
    std::map<std::string, class UFunction*> m_staticFunctions;
    std::vector<class UObject*> createdObjects_;

    template<typename T>
    auto GetTypeName() -> std::string;

    // Game-specific cached instances
    //    AGFxHUD_TA* hud;
    //    UGFxDataStore_X* dataStore;
    //    USaveData_TA* saveData;
    //    UOnlinePlayer_X* onlinePlayer;
};
