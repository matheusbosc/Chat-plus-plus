//
// Created by matheus on 1/27/26.
//

#include <iostream>
#include <string>

#include "server.h"
#include "ui.h"
#include "json_struct/json_struct.h"

std::atomic<bool> g_running{true};

int main() {

    //std::string windows_settings_path = "%APPDATA%/ChatPlusPlus/settings.json";
    //std::string linux_settings_path = "~/.config/YourApp/settings.json";
    //std::string macos_settings_path = "~/Library/Application Support/YourApp/settings.json";

    //ChatUI ui("settings.json");
    Server client;

    client.Init(false);

    if (client.start_server(8080) != 0) return -1;

    client.start_server_listener();

    while (g_running.load()) {

    }

    client.stop_server();

}

void handle_sigint(int) {
    g_running.store(false);
}
