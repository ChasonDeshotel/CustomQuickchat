#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_internal.h"
#include "lib/imgui/misc/cpp/imgui_stdlib.h"
#include "src/gui/SearchableCombo.h"
#include "src/gui/RangeSlider.h"

#include <limits.h>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <shellapi.h>

#include "json/include/nlohmann/json.hpp"
#include "SDK/SdkHeaders.hpp"

#include "logging.h"

#pragma comment(lib, "Shlwapi.lib")

namespace fs = std::filesystem;
using json = nlohmann::json;