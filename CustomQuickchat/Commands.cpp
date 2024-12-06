#include "pch.h"
#include "CustomQuickchat.h"



void CustomQuickchat::cmd_toggleEnabled(std::vector<std::string> args)
{
	CVarWrapper enabledCvar = GetCvar(Cvars::enabled);
	if (!enabledCvar) return;

	bool enabled = enabledCvar.getBoolValue();
	enabledCvar.setValue(!enabled);
}


void CustomQuickchat::cmd_listBindings(std::vector<std::string> args)
{
	// list button bindings
	auto controls = Instances.GetInstanceOf<UGFxData_Controls_TA>();
	if (!controls) {
		LOG("UGFxData_Controls_TA* is null!");
		return;
	}

	auto gpBindings = controls->GamepadBindings;
	auto pcBindings = controls->PCBindings;

	LOG("================ PC bindings =================");
	for (const auto& binding : pcBindings)
	{
		LOG("{}: {}", binding.Action.ToString(), binding.Key.ToString());
	}

	LOG("============== Gamepad bindings ==============");
	for (const auto& binding : gpBindings)
	{
		LOG("{}: {}", binding.Action.ToString(), binding.Key.ToString());
	}
}


void CustomQuickchat::cmd_exitToMainMenu(std::vector<std::string> args)
{
	auto shell = Instances.GetInstanceOf<UGFxShell_X>();
	if (!shell) return;

	shell->ExitToMainMenu();

	LOG("exited to main menu");
}


void CustomQuickchat::cmd_forfeit(std::vector<std::string> args)
{
	auto shell = Instances.GetInstanceOf<UGFxShell_TA>();
	if (!shell) return;

	shell->VoteToForfeit();

	LOG("voted to forfeit...");
}


void CustomQuickchat::cmd_test(std::vector<std::string> args)
{
#ifdef USE_SPEECH_TO_TEXT

	Websocket->SendEvent("test", { { "data", "test" } });

#endif // USE_SPEECH_TO_TEXT
}


void CustomQuickchat::cmd_test2(std::vector<std::string> args)
{
	if (args.size() < 2) return;

	UGFxData_Chat_TA* chat = Instances.GetInstanceOf<UGFxData_Chat_TA>();
	if (!chat)
	{
		LOG("UGFxData_Chat_TA* is null!");
		return;
	}

	auto shell = chat->Shell;
	if (!shell) {
		LOG("chat->Shell is null");
		return;
	}

	UGFxDataStore_X* dataStore = shell->DataStore;
	if (!dataStore) {
		LOG("shell->DataStore is null");
		return;
	}

	std::string test_label = "tester"

	dataStore->SetStringValue(L"ChatPresetMessages", std::stoi(args[1]), L"Label", Instances.FString(test_label));	// works

	LOG("set preset message label to '{}'", test_label);

	return;

	auto tables = dataStore->Tables;

	for (auto& table : tables)
	{
		if (table.Name.ToString() == "ChatPresetMessages")
		{
			for (auto& row : table.Rows)
			{
				auto vals = row.Values;
			}
		}
	}


	return;

	auto& preset_messages = chat->PresetMessages;
	LOG("PresetMessages size: {}", preset_messages.size());

	for (FChatPresetMessage& message : preset_messages)
	{
		LOG("--------------------------------------");
		LOG("Label: {}", message.Label.ToString());
		LOG("--------------------------------------");
		LOG("GroupIndex: {}", message.GroupIndex);
		LOG("Id: {}", message.Id.ToString());
		LOG("bTeam: {}", static_cast<bool>(message.bTeam));
	
		if (message.GroupIndex == 3)
		{
			message.Label = Instances.FString(test_label);
		}
	}

	chat->OnShellSet();
	chat->eventOnShellSet();
}