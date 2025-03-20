#include "Reflection.h"

auto
Reflection::FindStaticClass(const std::string& className) -> class UClass* {
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
Reflection::FindStaticFunction(const std::string& className) -> class UFunction* {
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
