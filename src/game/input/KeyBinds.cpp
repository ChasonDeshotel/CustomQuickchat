#include "KeyBinds.h"

void
KeyBinds::list(std::vector<std::string> args) {
    // list button bindings
    auto controls = Instances.GetInstanceOf<UGFxData_Controls_TA>();
    if (!controls) {
        LOG("UGFxData_Controls_TA* is null!");
        return;
    }

    auto gpBindings = controls->GamepadBindings;
    auto pcBindings = controls->PCBindings;

    LOG("================ PC bindings =================");
    for (const auto& binding : pcBindings) {
        LOG("{}: {}", binding.Action.ToString(), binding.Key.ToString());
    }

    LOG("============== Gamepad bindings ==============");
    for (const auto& binding : gpBindings) {
        LOG("{}: {}", binding.Action.ToString(), binding.Key.ToString());
    }
}

void
KeyBinds::ResetAllFirstButtonStates() {
    for (Binding& binding : Bindings) {
        binding.firstButtonState.Reset(epochTime);
    }
}

void
KeyBinds::InitKeyStates() {
    for (const std::string& keyName : possibleKeyNames) {
        keyStates[keyName] = false;
    }
}

void
KeyBinds::AddEmptyBinding() {
    Binding newBinding;
    Bindings.push_back(newBinding);
}

void
KeyBinds::AddEmptyVariationList() {
    VariationList list;
    Variations.push_back(list);
}

void
KeyBinds::DeleteBinding(int idx) {
    if (Bindings.empty())
        return;

    // erase binding at given index
    Bindings.erase(Bindings.begin() + idx);

    // reset selected binding index
    selectedBindingIndex = Bindings.empty() ? 0 : Bindings.size() - 1;

    // update JSON
    WriteBindingsToJson();
}
auto
KeyBinds::FindButtonIndex(const std::string& buttonName) -> int {
    auto it = std::find(possibleKeyNames.begin(), possibleKeyNames.end(), buttonName);
    if (it != possibleKeyNames.end()) {
        return std::distance(possibleKeyNames.begin(), it);
    } else {
        return 0;
    }
}

void
KeyBinds::UpdateBindingsData() {
    for (auto& binding : Bindings) {
        binding.UpdateKeywordAndTextEffect(keywordRegexPattern);
    }
}
