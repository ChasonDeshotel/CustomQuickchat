#include "InputHandler.h"
#include "GameState.h"

#include <ChatManager.h>
#include <TAGame_parameters.hpp>

InputHandler::InputHandler(std::function<std::shared_ptr<GameState>()> gameState,
    std::function<std::shared_ptr<ChatManager>()> chatManager)
  : gameState_(std::move(gameState))
  , chatManager_(std::move(chatManager)) {}

void
InputHandler::init() {
    // InitKeyStates();
}

void
InputHandler::KeyPress(ActorWrapper caller, void* params, std::string eventName) {
    auto gameState = gameState_();
    auto chatManager = chatManager_();

    // fixme: check if any binding for that key and exit early

    if (gameState->isGamePaused() || !gameState->isInGame()) {
        return;
    }

    if (gameState->isMatchEnded()) {
        if (chatManager->getDisablePostMatch()) {
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

                    actionHandler->performAction(binding);
                    return;
                }
            }
        } else if (keyEventType == EInputEvent::IE_Released)
            keyStates[keyName] = false; // update key state (for CheckCombination() to analyze a "snapshot" of all pressed buttons)
    }