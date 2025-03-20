#include "KeyHandler.h"

void
KeyHandler::KeyPress(ActorWrapper caller, void* params, std::string eventName) {
    // TODO: check if any binding for that key and exit early

    if (gamePaused || !inGameEvent) {
        return;
    }

    if (matchEnded) {
        auto disablePostMatchQuickchats_cvar = GetCvar(Cvars::disablePostMatchQuickchats);
        if (!disablePostMatchQuickchats_cvar || disablePostMatchQuickchats_cvar.getBoolValue())
            return;
    }

    auto* keyPressData = reinterpret_cast<UGameViewportClient_TA_execHandleKeyPress_Params*>(params);
    if (keyPressData == nullptr) {
        return;
    }

    std::string keyName = keyPressData->Key.ToString();
    auto keyEventType = static_cast<EInputEvent>(keyPressData->EventType);

    if (keyEventType == EInputEvent::IE_Pressed) {
        keyStates[keyName] = true; // update key state (for CheckCombination() to analyze a "snapshot" of all pressed buttons)

        // update state for tracking whether user is using gamepad or pc inputs
        using_gamepad = keyPressData->bGamepad;
        // LOG("Using gamepad: {}", using_gamepad);		// can uncomment for testing purposes, otherwise it clutters up the
        // console

        ButtonPress buttonPressEvent{ keyName, std::chrono::steady_clock::now() };

        // get min binding delay
        auto minBindingDelay_cvar = GetCvar(Cvars::minBindingDelay);
        if (!minBindingDelay_cvar)
            return;
        double minBindingDelay_raw = minBindingDelay_cvar.getFloatValue();
        auto minBindingDelay = std::chrono::duration<double>(minBindingDelay_raw);

        // get max sequence time window
        auto sequenceTimeWindow_cvar = GetCvar(Cvars::sequenceTimeWindow);
        if (!sequenceTimeWindow_cvar)
            return;
        double sequenceTimeWindow_raw = sequenceTimeWindow_cvar.getFloatValue();
        auto sequenceTimeWindow = std::chrono::duration<double>(sequenceTimeWindow_raw);

        // check if any bindings triggered
        for (Binding& binding : Bindings) {
            if (binding.enabled &&
                binding.ShouldBeTriggered(
                    buttonPressEvent, keyStates, lastBindingActivated, epochTime, minBindingDelay, sequenceTimeWindow)) {
                // reset/update data for all bindings
                lastBindingActivated = std::chrono::steady_clock::now();
                ResetAllFirstButtonStates();

                // activate binding action
                PerformBindingAction(binding);
                return;
            }
        }
    } else if (keyEventType == EInputEvent::IE_Released)
        keyStates[keyName] = false; // update key state (for CheckCombination() to analyze a "snapshot" of all pressed buttons)
}

void
CustomQuickchat::PerformBindingAction(const Binding& binding) {
    // processedChat starts out as the original raw chat string, and will get processed if it includes word variations or relevant
    // keywords (i.e. lastChat)
    std::string processed_chat = binding.chat;

    switch (binding.keyWord) {
        case EKeyword::SpeechToText:
        case EKeyword::SpeechToTextSarcasm:
        case EKeyword::SpeechToTextUwu:
            // clang-format off
            #if defined(USE_SPEECH_TO_TEXT)
            if (!attemptingSTT)
                StartSpeechToText(binding);
            else
                STTLog("Speech-to-text is already active!");
            #else
            no_speech_to_text_warning();
            #endif
            return;
            // clang-format on

        case EKeyword::BlastAll:
        case EKeyword::BlastCasual:
        case EKeyword::Blast1v1:
        case EKeyword::Blast2v2:
        case EKeyword::Blast3v3: SendChat(get_last_chatter_rank_str(binding.keyWord), binding.chatMode); return;
        case EKeyword::Forfeit: RunCommand(Commands::forfeit); return;
        case EKeyword::ExitToMainMenu: RunCommand(Commands::exitToMainMenu); return;

        // lastChat and word variations need to parse the chat string every time binding is triggered (but im prolly wrong, and theres
        // a way to eliminate the need)
        // ... the others only need to do it when the binding is created
        case EKeyword::LastChat:
        case EKeyword::LastChatSarcasm:
        case EKeyword::LastChatUwu:
        case EKeyword::WordVariation: processed_chat = process_keywords_in_chat_str(binding); break;
        default: break;
    }

    // send processed chat
    if (processed_chat.empty())
        return;
    SendChat(processed_chat, binding.chatMode);
}

void
CustomQuickchat::cmd_exitToMainMenu(std::vector<std::string> args) {
    auto shell = Instances.GetInstanceOf<UGFxShell_X>();
    if (!shell)
        return;

    shell->ExitToMainMenu();

    LOG("exited to main menu");
}

void
CustomQuickchat::cmd_forfeit(std::vector<std::string> args) {
    auto shell = Instances.GetInstanceOf<UGFxShell_TA>();
    if (!shell)
        return;

    shell->VoteToForfeit();

    LOG("voted to forfeit...");
}
