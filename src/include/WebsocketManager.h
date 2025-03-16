#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using websocketpp::connection_hdl;

class WebsocketClientManager
{
	using PluginClient = websocketpp::client<websocketpp::config::asio_client>;

public:
	WebsocketClientManager(std::function<void(json serverResponse)> response_callback, std::shared_ptr<bool> connecting_to_ws_server);
	~WebsocketClientManager() {};

	bool StartClient(int port);				// Connect to the WebSocket server
	bool StopClient();						// Disconnect from the WebSocket server

	void SendEvent(const std::string& eventName, const json& dataJson);
	void SetbUseBase64(bool bNewValue) { bUseBase64 = bNewValue; }
	bool IsConnectedToServer() { return is_connected; }

	std::string get_port_str() const;

	void set_connected_status(bool connected);

private:
	WebsocketClientManager() = delete;
	//~WebsocketClientManager() { StopClient(); };

	std::shared_ptr<bool> connecting_to_server;

	int port_number = 42069;
	std::string port_num_str = std::to_string(port_number);
	std::string server_uri = "ws://localhost:" + port_num_str;		// WebSocket server URI
	bool bUseBase64 = false;

	PluginClient ws_client;					// The WebSocket client instance
	connection_hdl ws_connection_handle;	// Handle for the active connection
	std::thread ws_client_thread;			// Thread for the WebSocket client
	bool is_connected = false;				// Whether the client is connected to the server

	std::function<void(json serverResponse)> handle_server_response;

	void OnWsOpen(connection_hdl hdl);
	void OnWsClose(connection_hdl hdl);
	void OnWsMessage(connection_hdl hdl, PluginClient::message_ptr msg);
};
