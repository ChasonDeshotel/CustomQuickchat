#include "pch.h"
#include "main.h"

CVarWrapper CustomQuickchat::RegisterCvar_Bool(const Cvars::CvarData& cvar, bool startingValue)
{
	std::string value = startingValue ? "1" : "0";

	return cvarManager->registerCvar(cvar.name, value, cvar.description, true, true, 0, true, 1);
}


CVarWrapper CustomQuickchat::RegisterCvar_String(const Cvars::CvarData& cvar, const std::string& startingValue)
{
	return cvarManager->registerCvar(cvar.name, startingValue, cvar.description);
}


CVarWrapper CustomQuickchat::RegisterCvar_Number(const Cvars::CvarData& cvar, float startingValue, bool hasMinMax, float min, float max)
{
	std::string numberStr = std::to_string(startingValue);

	if (hasMinMax)
	{
		return cvarManager->registerCvar(cvar.name, numberStr, cvar.description, true, true, min, true, max);
	}
	else
	{
		return cvarManager->registerCvar(cvar.name, numberStr, cvar.description);
	}
}


CVarWrapper CustomQuickchat::RegisterCvar_Color(const Cvars::CvarData& cvar, const std::string& startingValue)
{
	return cvarManager->registerCvar(cvar.name, startingValue, cvar.description);
}


CVarWrapper CustomQuickchat::GetCvar(const Cvars::CvarData& cvar)
{
	return cvarManager->getCvar(cvar.name);
}


void CustomQuickchat::RegisterCommand(const Cvars::CvarData& cvar, std::function<void(std::vector<std::string>)> callback)
{
	cvarManager->registerNotifier(cvar.name, callback, cvar.description, PERMISSION_ALL);
}


void CustomQuickchat::RunCommand(const Cvars::CvarData& command, float delaySeconds)
{
	if (delaySeconds == 0)
	{
		cvarManager->executeCommand(command.name);
	}
	else if (delaySeconds > 0)
	{
		gameWrapper->SetTimeout([this, command](GameWrapper* gw) {
			cvarManager->executeCommand(command.name);
		}, delaySeconds);
	}
}


void CustomQuickchat::RunCommandInterval(const Cvars::CvarData& command, int numIntervals, float delaySeconds, bool delayFirstCommand)
{
	if (!delayFirstCommand)
	{
		RunCommand(command);
		numIntervals--;
	}

	for (int i = 1; i <= numIntervals; i++)
	{
		RunCommand(command, delaySeconds * i);
	}
}
