cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0135 NEW) # Elimina el warning de JSON
project(PomoCli)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)


FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v5.0.0
)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)


FetchContent_MakeAvailable(ftxui json)

find_package(Threads REQUIRED)

# Obliga a CMake a poner el ejecutable en la raíz de la carpeta build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(pomodoro 
    src/main.cpp 
    src/ui.cpp
)

target_link_libraries(pomodoro PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
    nlohmann_json::nlohmann_json
    Threads::Threads
)
