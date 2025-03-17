#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

class PreEvent
{
protected:
	class UObject* PE_Caller;
	class UFunction* PE_Function;
	void* PE_Params; // pointer to params struct
	bool PE_Detour; // "If we should detour this event or not through Process Event, this is entirely up to us and not the game."
					// but not detoured isn't running through ProcessInternal? AFAIK? These comments weren't entirely accurate so.... ??

public:
	PreEvent();
	PreEvent(class UObject* caller, class UFunction* function, void* params);
	~PreEvent();

public:
	class UObject* Caller() const;
	template <typename T> T* GetCaller() const;
	class UFunction* Function() const;
	void* Params() const;
	template <typename T> T* GetParams() const;
	bool ShouldDetour() const;
	void SetDetour(bool bDetour);

public:
	PreEvent operator=(const PreEvent& other);
};

class PostEvent : public PreEvent
{
private:
	void* PE_Result; // result is unused after going through Process Event.

public:
	PostEvent();
	PostEvent(class UObject* caller, class UFunction* function, void* params, void* result);
	~PostEvent();

public:
	void* Result() const;
	template <typename T> T* GetResult() const;

public:
	PostEvent operator=(const PostEvent& other);
};

namespace Hooks
{
	void HUDPostRender(PreEvent& event);
	void HUDPostRenderPost(const PostEvent& event);
	void GameViewPortPostRender(PreEvent& event);
	void GFxDataMainMenuAdded(PreEvent& event);
	void PlayerControllerTick(PreEvent& event);
	void GameViewPortKeyPress(const PostEvent& event);
}

typedef void(*ProcessEventType)(class UObject*, class UFunction*, void*, void*); // Calling object, function, structure pointer with parameters, unused result

// Manages everything related to hooking functions from Process Event, as well as detouring.
class UEDetours
{
private:
	bool Detoured;
	ProcessEventType ProcessEvent;
	std::map<uint32_t, std::vector<std::function<void(PreEvent&)>>> PreHookedEvents; // Hooked functions internal integer and bound function.
	std::map<uint32_t, std::vector<std::function<void(const PostEvent&)>>> PostHookedEvents; // Hooked functions internal integer and bound function.
	std::vector<uint32_t> BlacklistedEvents; // Blacklisted functions internal integer.

public:
	UEDetours();
	~UEDetours();

public:
	void Initialize();
	void Shutdown();

public:
	bool IsDetoured() const;
	void AttachDetour(const ProcessEventType& detourFunction); // Redirects the process event virtual function to our own void, for us to manually process later to the typedef.
	void DetachDetour(); // Called by the deconstuctor, necessary for if your DLL gets intentionally (or unintentionally) unloaded before your game exits.
	void ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result); // Process event gets detoured to this function, then we manually proxy it through to "ProcessEvent".
	bool IsEventBlacklisted(uint32_t functionIndex) const;

public:
	void BlacklistEvent(const std::string& functionName);
	void WhitelistEvent(const std::string& functionName);

	std::vector<std::string> whitelist;
	std::vector<std::string> blacklist;

	void HookEventPre(const std::string& functionName, std::function<void(PreEvent&)> preHook);
	void HookEventPre(uint32_t functionIndex, std::function<void(PreEvent&)> preHook);
	void HookEventPost(const std::string& functionName, std::function<void(const PostEvent&)> postHook);
	void HookEventPost(uint32_t functionIndex, std::function<void(const PostEvent&)> postHook);

public:
	bool bMonitorEvents;
	bool bMonitorHUDPostRender;
	bool bMonitorHUDPostRenderPost;
	bool bMonitorPlayerControllerTick;
	bool bChatPressed;

	void MonitorEvents(bool state) { bMonitorEvents = state; }
	void MonitorHUDPostRender(bool state) { bMonitorHUDPostRender = state; }
	void MonitorHUDPostRenderPost(bool state) { bMonitorHUDPostRenderPost = state; }
	void MonitorPlayerControllerTick(bool state) { bMonitorPlayerControllerTick = state; }

	void ChatPressed(bool state) { bChatPressed = state; }
};

// Create a global instance that can be accessed from anywhere
extern UEDetours* g_UEDetours;
