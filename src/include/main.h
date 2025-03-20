#pragma once

#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "gui/Base.h"
#include "pch.h"

#include "Keys.h"
#include "Structs.h"
#include "TextEffects.h"

#include "version.h"

#include "Cvars.h"
#include "Events.h"
#include "Macros.h"
#include "gui/Tools.h"

// #define USE_SPEECH_TO_TEXT

constexpr auto plugin_version =
    stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#if !defined(USE_SPEECH_TO_TEXT)
constexpr auto pretty_plugin_version = "v" stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH);
#else
constexpr auto pretty_plugin_version =
    "v" stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "\t(with speech-to-text)";
#endif

class CustomQuickchat
  : public BakkesMod::Plugin::BakkesModPlugin
  , public SettingsWindowBase
  , public PluginWindowBase {

    void onLoad() override;
    void onUnload() override;

    void toggleEnabled(std::vector<std::string> args);

    static constexpr const char* keywordRegexPattern = R"(\[\[(.*?)\]\])";

    fs::path customQuickchatFolder;
    fs::path bindingsFilePath;
    fs::path variationsFilePath;

    static constexpr int MAX_KEYWORD_DEPTH = 10;

    void ResetAllFirstButtonStates();
    int FindButtonIndex(const std::string& buttonName);

    // commands
    void cmd_toggleEnabled(std::vector<std::string> args);
    void cmd_listBindings(std::vector<std::string> args);
    void cmd_list_custom_chat_labels(std::vector<std::string> args);
    void cmd_list_playlist_info(std::vector<std::string> args);

    // cvar change callbacks
    void changed_enabled(std::string cvarName, CVarWrapper updatedCvar);
    void changed_enableSTTNotifications(std::string cvarName, CVarWrapper updatedCvar);
    void changed_overrideDefaultQuickchats(std::string cvarName, CVarWrapper updatedCvar);
    void changed_blockDefaultQuickchats(std::string cvarName, CVarWrapper updatedCvar);
    void changed_useCustomChatTimeoutMsg(std::string cvarName, CVarWrapper updatedCvar);
    void changed_customChatTimeoutMsg(std::string cvarName, CVarWrapper updatedCvar);

    // hook callbacks
    void Event_KeyPressed(ActorWrapper caller, void* params, std::string eventName);
    void Event_GFxHUD_TA_ChatPreset(ActorWrapper caller, void* params, std::string eventName);
    void Event_OnPressChatPreset(ActorWrapper caller, void* params, std::string eventName);
    void Event_InitUIBindings(ActorWrapper caller, void* params, std::string eventName);
    void Event_ApplyChatSpamFilter(ActorWrapper caller, void* params, std::string eventName);
    void Event_NotifyChatDisabled(ActorWrapper caller, void* params, std::string eventName);
    void Event_OnChatMessage(ActorWrapper caller, void* params, std::string eventName);
    void Event_PushMenu(ActorWrapper caller, void* params, std::string eventName);
    void Event_PopMenu(ActorWrapper caller, void* params, std::string eventName);
    void Event_LoadingScreenStart(std::string eventName);

    // cvar helper stuff
    CVarWrapper RegisterCvar_Bool(const CvarData& cvar, bool startingValue);
    CVarWrapper RegisterCvar_String(const CvarData& cvar, const std::string& startingValue);
    CVarWrapper RegisterCvar_Number(const CvarData& cvar, float startingValue, bool hasMinMax = false, float min = 0, float max = 0);
    CVarWrapper RegisterCvar_Color(const CvarData& cvar, const std::string& startingValue);
    CVarWrapper GetCvar(const CvarData& cvar);

    void RegisterCommand(const CvarData& cvar, std::function<void(std::vector<std::string>)> callback);
    void RunCommand(const CvarData& command, float delaySeconds = 0);
    void RunCommandInterval(const CvarData& command, int numIntervals, float delaySeconds, bool delayFirstCommand = false);
};
