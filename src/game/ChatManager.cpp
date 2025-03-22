#include "ChatManager.h"
#include "GameStructs.h"
#include "ObjectProvider.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

std::shared_ptr<GameWrapper> gameWrapper;

void
ChatManager::initHooks() {
    gameWrapper->HookEventWithCaller<ActorWrapper>(
        GameFuncs::OnChatMessage, [this](ActorWrapper caller, void* params, std::string eventName) {
            this->onChatMessageCallback(caller, params, eventName);
            // if (removeTimestamps) {
            // this->removeTimestamps(caller, params, eventName);
            // }
        });

    gameWrapper->HookEventWithCaller<ActorWrapper>(
        GameFuncs::OnPressChatPreset, [this](ActorWrapper caller, void* params, std::string eventName) {
            this->onPressChatPresetCallback(caller, params, eventName);
        });

    gameWrapper->HookEventWithCaller<ActorWrapper>(
        GameFuncs::GFxHUD_TA_ChatPreset, [this](ActorWrapper caller, void* params, std::string eventName) {
            this->gfxHudChatPresetCallback(caller, params, eventName);
        });

    // the thing that tells you chat is disabled
    gameWrapper->HookEventWithCallerPost<ActorWrapper>(
        GameFuncs::ApplyChatSpamFilter, [this](ActorWrapper caller, void* params, std::string eventName) {
            this->applyChatSpamFilterCallback(caller, params, eventName);
        });

    gameWrapper->HookEventWithCaller<ActorWrapper>(
        GameFuncs::GFxHUD_TA_NotifyChatDisabled, [this](ActorWrapper caller, void* params, std::string eventName) {
            this->notifyChatDisabledCallback(caller, params, eventName);
        });

    gameWrapper->HookEvent(GameFuncs::SendChatPresetMessage, [this](std::string eventName) {
        // reset/update data for all bindings
        lastBindingActivated = std::chrono::steady_clock::now();
        // keyManager->ResetAllFirstButtonStates();
    });
}

void
ChatManager::sendMessage(const std::string& chat, EChatChannel chatMode) {
    if (!enabled || chat.empty()) {
        return;
    }

    UGFxData_Chat_TA* chatBox = GetInstanceOf<UGFxData_Chat_TA>();
    if (!chatBox) {
        // LOG("UGFxData_Chat_TA* is null!");
        return;
    }

    FString chatFStr = Instances.NewFString(chat);

    if (chatMode == EChatChannel::EChatChannel_Match) {
        chatBox->SendChatMessage(chatFStr, 0); // match (lobby) chat

        // fixme, log removed from signature
        if (true) {
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

void
ChatManager::gfxHudChatPresetCallback(ActorWrapper caller, void* params, std::string eventName) {
    if (!enabled) {
        return;
    }

    auto* Params = reinterpret_cast<AGFxHUD_TA_execChatPreset_Params*>(params);
    if (!Params) {
        return;
    };

    //    // get cvars
    //    auto overrideDefaultQuickchats_cvar = GetCvar(Cvars::overrideDefaultQuickchats);
    //    auto blockDefaultQuickchats_cvar = GetCvar(Cvars::blockDefaultQuickchats);
    //
    //    // block default quickchat if necessary
    //    if (overrideDefaultQuickchats_cvar.getBoolValue()) {
    //        auto currentTime = std::chrono::steady_clock::now();
    //
    //        auto blockQuickchatWindow = std::chrono::duration<double>(BLOCK_DEFAULT_QUICKCHAT_WINDOW);
    //
    //        if (currentTime <= (lastBindingActivated + blockQuickchatWindow)) {
    //            Params->Index = 420; // effectively blocks default quickchat from propagating
    //        }
    //    } else if (blockDefaultQuickchats_cvar.getBoolValue()) {
    //        Params->Index = 420; // effectively blocks default quickchat from propagating
    //    }
}

void
ChatManager::onPressChatPresetCallback(ActorWrapper Caller, void* Params, std::string eventName) {
    if (gameWrapper->IsInFreeplay()) {
        return;
    };

    //    auto overrideDefaultQuickchats_cvar = GetCvar(Cvars::overrideDefaultQuickchats);
    //
    //    if (!enabled_cvar || !overrideDefaultQuickchats_cvar)
    //        return;
    //    if (!enabled_cvar.getBoolValue() || !overrideDefaultQuickchats_cvar.getBoolValue())
    //        return;
    //
    //    auto caller = reinterpret_cast<UGFxData_Chat_TA*>(Caller.memory_address);
    //    if (!caller)
    //        return;
    //
    //    auto params = reinterpret_cast<UGFxData_Chat_TA_execOnPressChatPreset_Params*>(Params);
    //    if (!params)
    //        return;
    //
    //    apply_custom_qc_labels_to_ui(caller, params);
}

bool
ChatManager::CanSendMessage() const {
    return true;
}

void
ChatManager::applyChatSpamFilterCallback(ActorWrapper caller, void* params, std::string eventName) {
    if (!enabled) {
        return;
    }

    //    APlayerController_TA* pc = reinterpret_cast<APlayerController_TA*>(caller.memory_address);
    //    if (!pc)
    //        return;
    //
    //    auto disableChatTimeout_cvar = GetCvar(Cvars::disableChatTimeout);
    //    if (!disableChatTimeout_cvar)
    //        return;
    //    bool disableChatTimeout = disableChatTimeout_cvar.getBoolValue();
    //
    //    // effectively disables chat timeout (in freeplay)
    //    pc->ChatSpam.MaxValue = disableChatTimeout ? 420 : 4;   // default 4
    //    pc->ChatSpam.DecayRate = disableChatTimeout ? 69 : 1;   // default 1
    //    pc->ChatSpam.RiseAmount = disableChatTimeout ? 1 : 1.2; // default 1.2
}

void
ChatManager::notifyChatDisabledCallback(ActorWrapper caller, void* params, std::string eventName) {
    // gamePaused = false;

    //    AGFxHUD_TA* hud = reinterpret_cast<AGFxHUD_TA*>(caller.memory_address);
    //    if (!hud) { return; }
    //
    //    this->SetChatTimeoutMsg(chatTimeoutMsg, hud);
}

// remove chat timestamps
// void
// ChatManager::RemoveTimestamps(ActorWrapper caller, void* params, std::string eventName) {
//    if (Params == nullptr) { return; }
//    FGFxChatMessage* Params = reinterpret_cast<FGFxChatMessage*>(params);
//    Params->TimeStamp = "";
//}

void
ChatManager::onChatMessageCallback(ActorWrapper caller, void* params, std::string eventName) {
    // TODO: replace cvar with variables
    // this happens in the event loop = optimize for speed
    // probably applies to other functions as well
    //    auto removeTimestamps_cvar = GetCvar(Cvars::removeTimestamps);
    //    if (!removeTimestamps_cvar || !removeTimestamps_cvar.getBoolValue())
    //        return;
    //
    //    FGFxChatMessage* Params = reinterpret_cast<FGFxChatMessage*>(params);
    //    if (!Params)
    //        return;
    //
    //    Params->TimeStamp = "";
    //    // Params->TimeStamp = Instances.NewFString("");           // <--- works as well
    //    // Params->TimeStamp = L"";                                                        // <--- but... this causes crash upon
    //    entering a
    //    // match for some reason... i think
}

void
ChatManager::determine_quickchat_labels(UGFxData_Controls_TA* controls, bool log) {
    //    if (!controls) {
    //        controls = Instances.GetInstanceOf<UGFxData_Controls_TA>();
    //        if (!controls) {
    //            LOG("UGFxData_Controls_TA* is null!");
    //            return;
    //        }
    //    }
    //
    //    std::array<BindingKey, 4> preset_group_bindings;
    //
    //    // find/save key bindings for each preset group
    //    for (int i = 0; i < 4; i++) {
    //        for (const auto& binding : controls->PCBindings) {
    //            std::string action_name = binding.Action.ToString();
    //
    //            if (action_name != preset_group_names[i])
    //                continue;
    //
    //            preset_group_bindings[i].action = action_name;
    //            preset_group_bindings[i].pc_key = binding.Key.ToString();
    //            break;
    //        }
    //
    //        for (const auto& binding : controls->GamepadBindings) {
    //            std::string action_name = binding.Action.ToString();
    //
    //            if (action_name != preset_group_names[i])
    //                continue;
    //
    //            preset_group_bindings[i].action = action_name;
    //            preset_group_bindings[i].gamepad_key = binding.Key.ToString();
    //            break;
    //        }
    //    }
    //
    //    if (log) {
    //        for (int i = 0; i < 4; i++) {
    //            LOG("========== preset_group_bindings[{}] ==========", i);
    //            LOG("action: {}", preset_group_bindings[i].action);
    //            LOG("pc_key: {}", preset_group_bindings[i].pc_key);
    //            LOG("gamepad_key: {}", preset_group_bindings[i].gamepad_key);
    //        }
    //
    //        LOG("Bindings.size(): {}", Bindings.size());
    //    }
    //
    //    for (const auto& binding : Bindings) {
    //        if (binding.bindingType != EBindingType::Sequence || binding.buttons.size() < 2)
    //            continue;
    //
    //        const std::string& first_button = binding.buttons[0];
    //        const std::string& second_button = binding.buttons[1];
    //
    //        // for (const BindingKey& preset_binding : preset_group_bindings)
    //        for (int group_index = 0; group_index < 4; group_index++) {
    //            const BindingKey& group_key = preset_group_bindings[group_index];
    //
    //            // check if matches a pc binding
    //            if (first_button == group_key.pc_key) {
    //                for (int chat_index = 0; chat_index < 4; chat_index++) {
    //                    const BindingKey& chat_key = preset_group_bindings[chat_index];
    //
    //                    if (second_button != chat_key.pc_key)
    //                        continue;
    //
    //                    pc_qc_labels[group_index][chat_index] = Instances.NewFString(binding.chat);
    //                    break;
    //                }
    //            }
    //            // check if matches a gamepad binding
    //            else if (first_button == group_key.gamepad_key) {
    //                for (int chat_index = 0; chat_index < 4; chat_index++) {
    //                    const BindingKey& chat_key = preset_group_bindings[chat_index];
    //
    //                    if (second_button != chat_key.gamepad_key)
    //                        continue;
    //
    //                    gp_qc_labels[group_index][chat_index] = Instances.NewFString(binding.chat);
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //
    //    LOG("Quickchat labels updated...");
}

void
ChatManager::apply_all_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller) {
    //    if (!caller)
    //        return;
    //
    //    auto shell = caller->Shell;
    //    if (!shell)
    //        return;
    //
    //    auto ds = shell->DataStore;
    //    if (!ds)
    //        return;
    //
    //    const auto& groups_of_chat_labels = using_gamepad ? gp_qc_labels : pc_qc_labels;
    //
    //    for (int group_index = 0; group_index < 4; group_index++) {
    //        const auto& group_of_labels = groups_of_chat_labels.at(group_index);
    //
    //        for (int label_index = 0; label_index < 4; label_index++) {
    //            const auto& chat_label = group_of_labels[label_index];
    //            if (chat_label.empty())
    //                continue;
    //
    //            int ds_row_index = (group_index * 4) + label_index;
    //
    //            // idk how this would ever happen, but to be safe...
    //            if (ds_row_index < 0 || ds_row_index > 15) {
    //                LOG("[ERROR] UGFxDataRow_X index out of range: {}", ds_row_index);
    //                continue;
    //            }
    //
    //            ds->SetStringValue(L"ChatPresetMessages", ds_row_index, L"Label", chat_label);
    //        }
    //    }
    //
    //    LOG("Updated quickchat labels in UI");
}

void
ChatManager::apply_custom_qc_labels_to_ui(UGFxData_Chat_TA* caller, UGFxData_Chat_TA_execOnPressChatPreset_Params* params) {
    //    if (!caller || !params)
    //        return;
    //
    //    const int32_t& index = params->Index;
    //    if (index == 420)
    //        return;
    //
    //    auto shell = caller->Shell;
    //    if (!shell)
    //        return;
    //
    //    auto ds = shell->DataStore;
    //    if (!ds)
    //        return;
    //
    //    const auto& chat_labels = using_gamepad ? gp_qc_labels[index] : pc_qc_labels[index];
    //
    //    for (int i = 0; i < 4; i++) {
    //        const auto& chat_label = chat_labels[i];
    //        if (chat_label.empty())
    //            continue;
    //
    //        int ds_row_index = (index * 4) + i;
    //
    //        // this prevents the chats with index of 420 (aka default RL quickchats that have been overridden) from being included
    //        if (ds_row_index < 0 || ds_row_index > 15) {
    //            // the ds_row_index of chats that have been suppressed/overridden would be 1680-1683, so not exactly an error but
    //            still
    //            // skip
    //            if (ds_row_index >= 1680 && ds_row_index < 1684)
    //                continue;
    //
    //            LOG("[ERROR] UGFxDataRow_X index out of range: {}", ds_row_index); // anything else would be an error
    //            continue;
    //        }
    //
    //        ds->SetStringValue(L"ChatPresetMessages", ds_row_index, L"Label", chat_label);
    //    }
    //
    //    LOG("Applied quickchat labels to UI for {} group", preset_group_names[index]);
}

void
ChatManager::ResetChatTimeoutMsg() {
    chatTimeoutMsg = "Chat disabled for [Time] second(s).";
}

// void
// ChatManager::SetTimeoutMsg(const std::string& newMsg, AGFxHUD_TA* hud) {
//     //    if (!hud) {
//     //        hud = GetInstanceOf<AGFxHUD_TA>();
//     //        if (!hud)
//     //            return;
//     //    }
//     //
//     //    if (hud->ChatDisabledMessage.ToString() != newMsg) {
//     //        hud->ChatDisabledMessage = Instances.NewFString(newMsg); // overwrite ChatDisabledMessage
//     //        LOG("Set chat timeout message: {}", newMsg);
//     //    }
// }

// void
// ChatManager::cmd_list_custom_chat_labels(std::vector<std::string> args) {
//     //    determine_quickchat_labels(nullptr, true);
//     //
//     //    for (int i = 0; i < 4; i++) {
//     //        const auto& chat_label_arr = pc_qc_labels[i];
//     //
//     //        LOG("{}:", preset_group_names[i]);
//     //        for (int j = 0; j < 4; j++) {
//     //            LOG("[{}]\t{}", j, chat_label_arr.at(j).ToString());
//     //        }
//     //    }
//     //
//     //    for (int i = 0; i < 4; i++) {
//     //        const auto& chat_label_arr = gp_qc_labels[i];
//     //
//     //        LOG("{}:", preset_group_names[i]);
//     //        for (int j = 0; j < 4; j++) {
//     //            LOG("[{}]\t{}", j, chat_label_arr.at(j).ToString());
//     //        }
//     //    }
// }

// auto
// ChatManager::get_last_chat() -> std::string {
//     //    ChatData chat = LobbyInfo.get_last_chat_data();
//     //
//     //    if (chat.Message.empty()) {
//     //        LOG("[ERROR] Message is empty string from last chat data");
//     //        return std::string();
//     //    }
//     //
//     //    return chat.Message;
// }

// auto
// ChatManager::get_last_chatter_rank_str(EKeyword keyword) -> std::string {
//     //    ChatterRanks chatter_ranks = LobbyInfo.get_last_chatter_ranks();
//     //    if (chatter_ranks.playerName.empty()) {
//     //        LOG("[ERROR] ChatterRanks::playerName is empty string");
//     //        return std::string();
//     //    }
//     //
//     //    switch (keyword) {
//     //        case EKeyword::BlastAll: return chatter_ranks.get_all_ranks_str();
//     //        case EKeyword::BlastCasual: return chatter_ranks.get_playlist_rank_str(ERankPlaylists::Casual);
//     //        case EKeyword::Blast1v1: return chatter_ranks.get_playlist_rank_str(ERankPlaylists::Ones);
//     //        case EKeyword::Blast2v2: return chatter_ranks.get_playlist_rank_str(ERankPlaylists::Twos);
//     //        case EKeyword::Blast3v3: return chatter_ranks.get_playlist_rank_str(ERankPlaylists::Threes);
//     //        default: return std::string();
//     //    }
// }