#pragma once

namespace StringUtil {

inline FString
ToFString(const std::string& str) {
    return FString(str.c_str());
}

inline FString
ToFString(const char* str) {
    return FString(str);
}

inline FString
ToFString(const std::wstring& str) {
    return FString(str.c_str());
}

}