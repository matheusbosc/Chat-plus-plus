//
// Created by matheus on 1/27/26.
//

#include "ui.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include "ftxui/component/component_options.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include "ftxui/component/captured_mouse.hpp"

using namespace ftxui;

void ChatUI::push_message(const std::string& msg) {
    messages.push_back(msg);
    // optionally trigger a redraw if needed
}

void ChatUI::PrepareConnection(std::string& tempPort) {
    if (username.empty()) {
        return;
    }

    if (room.empty()) {
        room = std::abs(rand());
    }

    if (ipAddress.empty()) {
        ipAddress = "127.0.0.1";
    }

    if (tempPort.empty()) {
        port = 8080;
    } else {
        try {
            unsigned long value = std::stoul(tempPort);
            if (value <= 65535) {
                port = static_cast<uint16_t>(value);
            } else {
                return;
            }
        } catch (const std::exception& e) {
            return;
        }
    }

    if (on_join) {
        on_join(username, room, ipAddress, port);
    }

    state = UIState::Chat;
}

void ChatUI::Quit(ScreenInteractive& screen) {

    if (on_quit) {
        on_quit(screen);
    }
}

void ChatUI::run() {

    auto screen = ScreenInteractive::Fullscreen();

    std::string tempPort;

    Component input_username = Input(&username, "name");
    Component input_room_code = Input(&room, "leave empty for random");
    Component input_address = Input(&ipAddress, "leave empty for default");

    Component input_port = Input(&tempPort, "leave empty for default");
        input_port |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });

    auto spacer = Renderer([] { return text("") | flex; });

    auto container =
    Container::Vertical({

        // Header
        Container::Horizontal({
            spacer | flex,
            Button("Quit", [&] {Quit(screen);}, ButtonOption::Ascii())
        }) | border,

        // Content
        Container::Vertical({
            Container::Horizontal({
                spacer | flex,
            }) | flex,
            Container::Horizontal({
                spacer | flex,
                Container::Vertical({

                    // Title
                    Container::Horizontal({
                        spacer | flex,
                        Renderer([] { return text("Chat++ by Matheus") ; }),
                        spacer | flex,
                    }),

                    Renderer([] { return separator(); }),

                    spacer | size(HEIGHT, EQUAL, Dimension::Full().dimy * 0.025f),

                    // Input Fields
                    Container::Horizontal({
                        Renderer([] { return text("Username  : "); }),
                        input_username,
                    }),
                    Container::Horizontal({
                        Renderer([] { return text("Room Code : "); }),
                        input_room_code,
                    }),
                    Container::Horizontal({
                        Renderer([] { return text("Address   : "); }),
                        input_address,
                    }),
                    Container::Horizontal({
                        Renderer([] { return text("Port      : "); }),
                        input_port,
                    }),

                    spacer | size(HEIGHT, EQUAL, Dimension::Full().dimy * 0.025f),

                    Renderer([] { return separatorCharacter("*"); }),

                    spacer | size(HEIGHT, EQUAL, Dimension::Full().dimy * 0.025f),


                    // Connect Button
                    Container::Horizontal({
                        spacer | flex,
                        Button("Connect", [&] {PrepareConnection(tempPort);}, ButtonOption::Ascii()) ,
                        spacer | flex,
                    }),

                }) | size(WIDTH, EQUAL, Dimension::Full().dimx * 0.36f) | border,
                spacer | flex,
            }) | xflex,
            Container::Horizontal({
                spacer | flex,
            }) | flex,

        }) | border | flex,

        // Footer
        Container::Horizontal({
            spacer | flex,
            Renderer([] { return text("Made by Matheus Boscariol"); }),
            spacer | flex,

        }) | border,
    }) | flex;


    screen.Loop(container);
}
