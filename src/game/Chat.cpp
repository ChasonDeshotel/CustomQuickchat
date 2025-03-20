#include "Chat.h"
#include "LogGlobal.h"

Chat::Chat() {}

void
Chat::initHooks() {
    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GFxData_Chat_TA.OnChatMessage",
        [this](ActorWrapper caller, void* params, std::string eventName) { this->onChatMessage(caller, params, eventName); });

    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GFxData_Chat_TA.OnPressChatPreset",
        [this](ActorWrapper caller, void* params, std::string eventName) { this->onPressChatPreset(caller, params, eventName); });

    // Other chat-related hooks...
}

void
Chat::SetTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud) {
    if (!hud) {
        hud = GetInstanceOf<AGFxHUD_TA>();
        if (!hud)
            return;
    }

    if (hud->ChatDisabledMessage.ToString() != newMsg) {
        hud->ChatDisabledMessage = Instances.NewFString(newMsg); // overwrite ChatDisabledMessage
        LOG("Set chat timeout message: {}", newMsg);
    }
}

void
Chat::SendChat(const std::string& chat, EChatChannel chatMode, bool log) {
    UGFxData_Chat_TA* chatBox = GetInstanceOf<UGFxData_Chat_TA>();
    if (!chatBox) {
        LOG("UGFxData_Chat_TA* is null!");
        return;
    }

    FString chatFStr = Instances.NewFString(chat);

    if (chatMode == EChatChannel::EChatChannel_Match) {
        chatBox->SendChatMessage(chatFStr, 0); // match (lobby) chat

        if (log) {
            LOG("Sent chat: '{}'", chat);
        }
    } else if (chatMode == EChatChannel::EChatChannel_Team) {
        chatBox->SendTeamChatMessage(chatFStr, 0); // team chat

        if (log) {
            LOG("Sent chat: [Team] '{}'", chat);
        }
    } else if (chatMode == EChatChannel::EChatChannel_Party) {
        chatBox->SendPartyChatMessage(chatFStr, 0); // party chat

        if (log) {
            LOG("Sent chat: [Party] '{}'", chat);
        }
    }
}
