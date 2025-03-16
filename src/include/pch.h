#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

//#define IMGUI_DEFINE_MATH_OPERATORS
//#include "imgui_bm/imgui.h"
//#include "imgui_bm/imgui_internal.h"
//#include "imgui_bm/misc/cpp/imgui_stdlib.h"
//#include "gui/SearchableCombo.h"
//#include "gui/RangeSlider.h"

#include <limits.h>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <shellapi.h>

#include "SDK/SdkHeaders.hpp"
#include "Instances.h"
#include "Utils.h"
#include "WebsocketManager.h"

#include <nlohmann/json.hpp>

#include "logging.h"

#pragma comment(lib, "Shlwapi.lib")

namespace fs = std::filesystem;
using json = nlohmann::json;
