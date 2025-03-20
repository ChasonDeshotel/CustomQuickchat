#include "main.h"
#include "pch.h"

BAKKESMOD_PLUGIN(CustomQuickchat, "Custom Quickchat", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void
CustomQuickchat::toggleEnabled(std::vector<std::string> args) {
    CVarWrapper enabledCvar = GetCvar(Cvars::enabled);
    if (!enabledCvar)
        return;

    bool enabled = enabledCvar.getBoolValue();
    enabledCvar.setValue(!enabled);
}

void
CustomQuickchat::onLoad() {
    _globalCvarManager = cvarManager;

    if (!EngineValidator.Init()) {
        return;
    }

    // bools
    auto enabled_cvar = RegisterCvar_Bool(Cvars::enabled, true);
    auto overrideDefaultQuickchats_cvar = RegisterCvar_Bool(Cvars::overrideDefaultQuickchats, true);
    auto blockDefaultQuickchats_cvar = RegisterCvar_Bool(Cvars::blockDefaultQuickchats, false);
    auto disablePostMatchQuickchats_cvar = RegisterCvar_Bool(Cvars::disablePostMatchQuickchats, false);
    auto disableChatTimeout_cvar = RegisterCvar_Bool(Cvars::disableChatTimeout, true);
    auto useCustomChatTimeoutMsg_cvar = RegisterCvar_Bool(Cvars::useCustomChatTimeoutMsg, false);
    auto removeTimestamps_cvar = RegisterCvar_Bool(Cvars::removeTimestamps, true);
    auto enableSTTNotifications_cvar = RegisterCvar_Bool(Cvars::enableSTTNotifications, true);
    auto autoCalibrateMic_cvar = RegisterCvar_Bool(Cvars::autoCalibrateMic, true);
    auto user_chats_in_last_chat_cvar = RegisterCvar_Bool(Cvars::user_chats_in_last_chat, true);
    auto teammate_chats_in_last_chat_cvar = RegisterCvar_Bool(Cvars::teammate_chats_in_last_chat, true);
    auto quickchats_in_last_chat_cvar = RegisterCvar_Bool(Cvars::quickchats_in_last_chat, true);
    auto party_chats_in_last_chat_cvar = RegisterCvar_Bool(Cvars::party_chats_in_last_chat, true);
    auto team_chats_in_last_chat_cvar = RegisterCvar_Bool(Cvars::team_chats_in_last_chat, true);

    // numbers
    auto micEnergyThreshold_cvar = RegisterCvar_Number(Cvars::micEnergyThreshold, 420);
    auto sequenceTimeWindow_cvar = RegisterCvar_Number(Cvars::sequenceTimeWindow, 2, true, 0, 10);
    auto minBindingDelay_cvar = RegisterCvar_Number(Cvars::minBindingDelay, 0.05, true, 0, 1);
    auto notificationDuration_cvar = RegisterCvar_Number(Cvars::notificationDuration, 3, true, 1.5, 10);
    auto beginSpeechTimeout_cvar = RegisterCvar_Number(Cvars::beginSpeechTimeout, 3, true, 1.5, 10);
    auto speechProcessingTimeout_cvar = RegisterCvar_Number(Cvars::speechProcessingTimeout, 10, true, 3, 500);
    auto micCalibrationTimeout_cvar = RegisterCvar_Number(Cvars::micCalibrationTimeout, 10, true, 3, 20);
    auto websocket_port_cvar = RegisterCvar_Number(Cvars::websocket_port, 8003, true, 0, 65535);

    // strings
    auto customChatTimeoutMsg_cvar = RegisterCvar_String(Cvars::customChatTimeoutMsg, "Wait [Time] seconds lil bro");

    // cvar change callbacks
    enabled_cvar.addOnValueChanged(std::bind(&CustomQuickchat::changed_enabled, this, std::placeholders::_1, std::placeholders::_2));

    enableSTTNotifications_cvar.addOnValueChanged(
        std::bind(&CustomQuickchat::changed_enableSTTNotifications, this, std::placeholders::_1, std::placeholders::_2));

    overrideDefaultQuickchats_cvar.addOnValueChanged(
        std::bind(&CustomQuickchat::changed_overrideDefaultQuickchats, this, std::placeholders::_1, std::placeholders::_2));

    blockDefaultQuickchats_cvar.addOnValueChanged(
        std::bind(&CustomQuickchat::changed_blockDefaultQuickchats, this, std::placeholders::_1, std::placeholders::_2));

    useCustomChatTimeoutMsg_cvar.addOnValueChanged(
        std::bind(&CustomQuickchat::changed_useCustomChatTimeoutMsg, this, std::placeholders::_1, std::placeholders::_2));

    customChatTimeoutMsg_cvar.addOnValueChanged(
        std::bind(&CustomQuickchat::changed_customChatTimeoutMsg, this, std::placeholders::_1, std::placeholders::_2));

    RegisterCommand(Commands::toggleEnabled, std::bind(&CustomQuickchat::cmd_toggleEnabled, this, std::placeholders::_1));
    RegisterCommand(Commands::listBindings, std::bind(&CustomQuickchat::cmd_listBindings, this, std::placeholders::_1));
    RegisterCommand(
        Commands::list_custom_chat_labels, std::bind(&CustomQuickchat::cmd_list_custom_chat_labels, this, std::placeholders::_1));
    RegisterCommand(Commands::list_playlist_info, std::bind(&CustomQuickchat::cmd_list_playlist_info, this, std::placeholders::_1));
    RegisterCommand(Commands::exitToMainMenu, std::bind(&CustomQuickchat::cmd_exitToMainMenu, this, std::placeholders::_1));
    RegisterCommand(Commands::forfeit, std::bind(&CustomQuickchat::cmd_forfeit, this, std::placeholders::_1));

    LobbyInfo.Initialize(gameWrapper);

    // make sure JSON files are good to go, then read them to update data
    GetFilePaths();
    CheckJsonFiles();
    ReadDataFromJson();
    gui_footer_init();

#ifdef USE_SPEECH_TO_TEXT
    ClearSttErrorLog();
    start_websocket_stuff(true);
#endif

    InitKeyStates();
    PreventGameFreeze();

    inGameEvent = gameWrapper->IsInFreeplay() || gameWrapper->IsInGame() || gameWrapper->IsInOnlineGame();

    LOG("CustomQuickchat loaded! :)");
}

void
CustomQuickchat::onUnload() {
    WriteBindingsToJson(); // just to make sure any unsaved changes are saved before exiting

#ifdef USE_SPEECH_TO_TEXT
    Websocket->StopClient();
    stop_websocket_server();

#endif // USE_SPEECH_TO_TEXT
}
