#include "pch.h"
#include "CustomQuickchat.h"
#include "GuiBase.h"


int CustomQuickchat::selectedBindingIndex = 0;

void CustomQuickchat::RenderSettings() {
	CVarWrapper chatsOnCvar = cvarManager->getCvar("customQuickchat_chatsOn");
	CVarWrapper macroTimeWindowCvar = cvarManager->getCvar("customQuickchat_macroTimeWindow");


	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::TextColored(ImVec4(1, 0, 1, 1), "Plugin made by SSLow");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Separator();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (!chatsOnCvar) { return; }
	bool chatsOn = chatsOnCvar.getBoolValue();
	if (ImGui::Checkbox("Use custom quickchats", &chatsOn)) {
		cvarManager->executeCommand("customQuickchat_toggle");
	}

	if (chatsOn) {
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// macro time window slider
		float macroTimeWindow = macroTimeWindowCvar.getFloatValue();
		ImGui::SliderFloat("button sequence time window", &macroTimeWindow, 0.0f, 10.0f, "%.1f seconds");
		macroTimeWindowCvar.setValue(macroTimeWindow);
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// open bindings window button
		if (ImGui::Button("Open Bindings Menu", ImVec2(0, 0))) {
			gameWrapper->Execute([this](GameWrapper* gw) {
				cvarManager->executeCommand("togglemenu " + GetMenuName());
				});
		}
	}
}


void CustomQuickchat::RenderWindow() {


	RenderAllBindings();
	ImGui::SameLine();
	RenderBindingDetails();
}

void CustomQuickchat::RenderAllBindings() {
	if (ImGui::BeginChild("##BindingsList", ImVec2(300, 0), true)) {

		ImGui::TextUnformatted("Bindings:");
		ImGui::Separator();

		for (int i = 0; i < Bindings.size(); i++) {
			Binding binding = Bindings[i];
			if (ImGui::Selectable((binding.chat + "##" + std::to_string(i)).c_str(), i == selectedBindingIndex)) {
				selectedBindingIndex = i;
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Add New Binding", ImVec2(-1, 0))) {

			AddEmptyBinding();

			selectedBindingIndex = Bindings.empty() ? 0 : Bindings.size() - 1;
			
		}
	}
	ImGui::EndChild();
}

void CustomQuickchat::RenderBindingDetails() {
	if (ImGui::BeginChild("##BindingsView", ImVec2(0, 0), true)) {

		if (Bindings.empty()) {
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::TextUnformatted("add a binding...");

			ImGui::EndChild();
			return;
		}

		Binding selectedBinding = Bindings[selectedBindingIndex];

		// binding display section title
		ImGui::TextUnformatted(selectedBinding.chat.c_str());
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Chat:");

		ImGui::Spacing();
		ImGui::Spacing();

		// binding chat
		std::string bindingChat = selectedBinding.chat;
		ImGui::InputTextWithHint("chat", "let me cook", &bindingChat);
		Bindings[selectedBindingIndex].chat = bindingChat;

		ImGui::Spacing();
		ImGui::Spacing();

		// binding chat mode
		ImGui::SearchableCombo("chat mode", &Bindings[selectedBindingIndex].chatMode, possibleChatModes, "", "");
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "How it's triggered:");

		ImGui::Spacing();
		ImGui::Spacing();

		// binding type
		ImGui::SearchableCombo("binding type", &Bindings[selectedBindingIndex].typeNameIndex, possibleBindingTypes, "", "");
		
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		
		if (possibleBindingTypes[Bindings[selectedBindingIndex].typeNameIndex] == "button sequence") {
			ImGui::Text("Note: button sequence bindings only use 2 buttons");
			ImGui::Spacing();
			ImGui::Spacing();
		}

		// binding buttons
		for (int i = 0; i < selectedBinding.buttonNameIndexes.size(); i++) {

			// button dropdown
			std::string label = "Button " + std::to_string(i + 1);
			ImGui::SearchableCombo(label.c_str(), &Bindings[selectedBindingIndex].buttonNameIndexes[i], possibleKeyNames, "", "");

			ImGui::SameLine();

			// remove button
			std::string removeLine = "Remove##" + std::to_string(i + 1);
			if (ImGui::Button(removeLine.c_str())) {
				Bindings[selectedBindingIndex].buttonNameIndexes.erase(Bindings[selectedBindingIndex].buttonNameIndexes.begin() + i);
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (!((possibleBindingTypes[Bindings[selectedBindingIndex].typeNameIndex] == "button sequence") && (Bindings[selectedBindingIndex].buttonNameIndexes.size() >= 2))) {

			// add new button
			if (ImGui::Button("Add New Button")) {
				Bindings[selectedBindingIndex].buttonNameIndexes.push_back(0);
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Save")) {
			WriteBindingsToJson();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// delete binding button
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		if (ImGui::Button("Delete Binding", ImVec2(0, 0))) {
			ImGui::PopStyleColor(3);
			ImGui::PopID();
			DeleteBinding(selectedBindingIndex);
		}
		else {
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

	}

	ImGui::EndChild();
}

