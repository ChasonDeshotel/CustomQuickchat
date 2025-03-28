#pragma once
#include <random>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "logging.h"
#include "SDK/GameDefines.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;

namespace Format
{
	// ----------------------- custom -------------------------

	FString ToFString(const std::string& str);

	wchar_t* ToWcharStringOld(const std::string& str);	// can handle strings with braces just fine

	wchar_t* ToWcharString(const std::string& str);	// can handle special characters

	std::string ToHexString(uintptr_t address);
	std::string ToHexString(int32_t decimal_val, int32_t min_hex_digits);

	FName ToFName(const std::string& str);

	std::string ToASCIIString(std::string str);

	std::vector<std::string> SplitStrByNewline(const std::string& input);
	std::vector<std::string> SplitStr(const std::string& str, const std::string& delimiter);

	std::string EscapeBraces(const std::string& str);

	std::string GenRandomString(int length);

	// --------------------------------------------------------


	std::string ToLower(std::string str);
	void ToLowerInline(std::string& str);
	std::string RemoveAllChars(std::string str, char character);
	void RemoveAllCharsInline(std::string& str, char character);

	bool IsStringHexadecimal(std::string str);

	std::string ToHex(void* address, bool bNotation = true);
	std::string ToHex(uint64_t decimal, size_t width, bool bNotation = true);
	uint64_t ToDecimal(const std::string& hexStr);
	std::string ToDecimal(uint64_t hex, size_t width);

	std::string ColorToHex(float colorsArray[3], bool bNotation);

	uint64_t HexToDecimal(const std::string& hexStr);
	uintptr_t HexToIntPointer(const std::string& hexStr);
}

namespace Files
{
	void FindPngImages(const fs::path& directory, std::unordered_map<std::string, fs::path>& imageMap);
	void OpenFolder(const fs::path& folderPath);

	std::string get_text_content(const fs::path& file_path);
	json get_json(const fs::path& file_path);
	bool write_json(const fs::path& file_path, const json& j);

	void FilterLinesInFile(const fs::path& filePath, const std::string& startString);
	std::string GetCommandOutput(const char* cmd);
	std::string CleanPathStr(const std::string& path);
}

namespace Process
{
	struct ProcessHandles
	{
		HANDLE hProcess =	NULL;
		HANDLE hThread =	NULL;
	};

	struct CreateProcessResult
	{
		DWORD status_code = ERROR_SUCCESS;
		ProcessHandles handles;
	};

	void close_handle(HANDLE h);
	void terminate(HANDLE h);
	void terminate_created_process(ProcessHandles& pi);

	CreateProcessResult create_process_from_command(const std::string& command);
}
