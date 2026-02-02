# Chat++/apps/server/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)

FetchContent_MakeAvailable(ftxui)

# Define the executable target
add_executable(server
        src/server/main.cpp
        src/server/server.cpp
        src/server/ui.cpp
)

# Include directories for headers
target_include_directories(server
        PRIVATE
        src/server
)

# Link against dependencies
target_link_libraries(server
        PRIVATE
        common_lib
        json_struct
        nlohmann

        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)