#pragma once

struct ActiveSTTAttempt {
    std::string attemptID;
    Binding binding;
};

struct SpeechToTextResult {
    bool success = false;
    bool error = false;
    std::string outputStr;
};

struct MicCalibrationResult {
    bool success = false;
    bool error = false;
    int energyThreshold = 420;
    std::string errorMsg;
};

// constants
static constexpr float PROBE_JSON_FREQUENCY = 0.2f; // in seconds

// thread-safe flags
std::atomic<bool> attemptingSTT = false;
std::atomic<bool> calibratingMicLevel = false;

// filepaths
fs::path speechToTextExePath;
fs::path speechToTextJsonPath;
fs::path speechToTextErrorLogPath;

// mutable values
ActiveSTTAttempt Active_STT_Attempt;

// websocket stuff
static constexpr float START_WS_CLIENT_DELAY = 5.0f; // in seconds

std::atomic<bool> connecting_to_ws_server{ false };
std::shared_ptr<WebsocketClientManager> Websocket = nullptr;
Process::ProcessHandles stt_python_server_process;

void
start_websocket_stuff(bool onLoad = false);
bool
start_websocket_server();
void
stop_websocket_server();
void
process_ws_response(const json& response);

// speech-to-text
void
StartSpeechToText(const Binding& binding);
json
generate_data_for_STT_attempt();
void
process_STT_result(const json& response_data);

// mic calibration
void
CalibrateMicrophone();
json
generate_data_for_mic_calibration_attempt();
void
process_mic_calibration_result(const json& response_data);

// misc
std::string
generate_STT_attempt_id();
void
ClearSttErrorLog();

std::string
CreateCommandString(const fs::path& executablePath, const std::vector<std::string>& args);
void
STTLog(const std::string& message);
