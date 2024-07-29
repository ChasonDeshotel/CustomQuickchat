#include "pch.h"
#include "CustomQuickchat.h"



void CustomQuickchat::enabled_Changed(std::string cvarName, CVarWrapper updatedCvar)
{
	bool enabled = updatedCvar.getBoolValue();

	std::string msg = "Custom quickchats turned " + std::string(enabled ? "ON" : "OFF");

	gameWrapper->Execute([this, msg](GameWrapper* gw) {

			Instances.SpawnNotification("Custom Quickchat", msg, 3);

		});

}


void CustomQuickchat::enableSTTNotifications_Changed(std::string cvarName, CVarWrapper updatedCvar)
{
	bool enableSTTNotifications = updatedCvar.getBoolValue();

	std::string msg = "Speech-To-Text notifications turned " + std::string(enableSTTNotifications ? "ON" : "OFF");

	LOG(msg);
}