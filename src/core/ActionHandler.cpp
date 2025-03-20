#include "ActionHandler.h"

void
ActionHandler::PerformBindingAction(const Binding& binding) {
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
    chatManager->SendMessage(processed_chat, binding.chatMode);
}

void
ActionHandler::exitToMainMenu(std::vector<std::string> args) {
    auto shell = Instances.GetInstanceOf<UGFxShell_X>();
    if (!shell) {
        return;
    }
    shell->ExitToMainMenu();
    LOG("exited to main menu");
}

void
ActionHandler::forfeit(std::vector<std::string> args) {
    auto shell = Instances.GetInstanceOf<UGFxShell_TA>();
    if (!shell) {
        return;
    }

    // u a bitch
    shell->VoteToForfeit();

    LOG("voted to forfeit...");
}
