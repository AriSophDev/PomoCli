cmake_minimum_required(VERSION 3.14)
project(PomoCli)

# 1. Configuración de C++ (Usamos C++17 para compatibilidad con FTXUI)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 2. Gestión de Dependencias (Descarga automática de FTXUI)
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0  # Versión estable
)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

# Esto descarga y configura las librerías en la carpeta 'build'
FetchContent_MakeAvailable(ftxui json)

# 3. Definición del Ejecutable
# IMPORTANTE: Aquí debes listar todos tus archivos .cpp
find_package(Threads REQUIRED)

add_executable(pomodoro 
    src/main.cpp 
    src/ui.cpp
)

# 4. Enlace de Librerías
# Esto le dice al compilador dónde encontrar las funciones de FTXUI,
# JSON y añade el soporte para hilos (Threads) que necesita tu timer.
target_link_libraries(pomodoro PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
    nlohmann_json::nlohmann_json
    Threads::Threads
)

