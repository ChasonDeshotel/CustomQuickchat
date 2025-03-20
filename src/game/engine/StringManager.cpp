#include "StringManager.h"

auto
StringManager::CreateFString(const std::string& str) -> FString {
    // have the game create a new FString using UE, rather than using new wchar_t* directly which causes crashes
    return UObject::RepeatString(Format::ToWideString(str).data(), 1);
}

auto
StringManager::CreateFString(const FString& old) -> FString {
    // have the game create a new FString using UE, rather than using new wchar_t* directly which causes crashes
    return UObject::RepeatString(old, 1);
}

auto
StringManager::CreateFName(const std::string& str) -> FName {
    return FName(Format::ToWideString(str).data());
}
