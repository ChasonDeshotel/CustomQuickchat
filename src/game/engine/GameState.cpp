#include "GameState.h"

gameWrapper->HookEventPost(Events::LoadingScreenStart,
    std::bind(&CustomQuickchat::Event_LoadingScreenStart, this, std::placeholders::_1));

gameWrapper->HookEventPost(Events::MatchEnded, [this](std::string eventName) { matchEnded = true; });
gameWrapper->HookEventPost(Events::EnterStartState, [this](std::string eventName) { inGameEvent = true; });

void
GameState::setPaused(ActorWrapper caller, void* params, std::string eventName) {
    UGFxData_MenuStack_TA_execPushMenu_Params* Params = reinterpret_cast<UGFxData_MenuStack_TA_execPushMenu_Params*>(params);
    if (!Params) {
        return;
    }

    if (Params->MenuName.ToString() == "MidGameMenuMovie") {
        gamePaused = true;
    }
}

void
GameState::setUnpaused(ActorWrapper caller, void* params, std::string eventName) {
    UGFxData_MenuStack_TA_execPopMenu_Params* Params = reinterpret_cast<UGFxData_MenuStack_TA_execPopMenu_Params*>(params);
    if (!Params) {
        return;
    }

    if (Params->MenuName.ToString() == "MidGameMenuMovie") {
        gamePaused = false;
    }
}

void
GameState::loadingScreenStart(std::string eventName) {
    gamePaused = false;
    matchEnded = false;
    inGameEvent = false;

    // reset all "pressed" buttons (to fix bug of bindings mistakenly firing bc a key's state is stuck in "pressed" mode upon joining a
    // game/freeplay)
    for (auto& [key, state] : keyStates) {
        state = false;
    }
}
