#include "pch.h"
#include "main.h"

auto CustomQuickchat::RegisterCvar_Bool(const CvarData& cvar, bool startingValue) -> CVarWrapper
{
	std::string value = startingValue ? "1" : "0";

	return cvarManager->registerCvar(cvar.name, value, cvar.description, true, true, 0, true, 1);
}


auto CustomQuickchat::RegisterCvar_String(const CvarData& cvar, const std::string& startingValue) -> CVarWrapper
{
	return cvarManager->registerCvar(cvar.name, startingValue, cvar.description);
}


auto CustomQuickchat::RegisterCvar_Number(const CvarData& cvar, float startingValue, bool hasMinMax, float min, float max) -> CVarWrapper
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


auto CustomQuickchat::RegisterCvar_Color(const CvarData& cvar, const std::string& startingValue) -> CVarWrapper
{
	return cvarManager->registerCvar(cvar.name, startingValue, cvar.description);
}


auto CustomQuickchat::GetCvar(const CvarData& cvar) -> CVarWrapper
{
	return cvarManager->getCvar(cvar.name);
}


void CustomQuickchat::RegisterCommand(const CvarData& cvar, std::function<void(std::vector<std::string>)> callback)
{
	cvarManager->registerNotifier(cvar.name, callback, cvar.description, PERMISSION_ALL);
}


void CustomQuickchat::RunCommand(const CvarData& command, float delaySeconds)
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


void CustomQuickchat::RunCommandInterval(const CvarData& command, int numIntervals, float delaySeconds, bool delayFirstCommand)
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
