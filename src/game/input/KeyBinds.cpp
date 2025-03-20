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