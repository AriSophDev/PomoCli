cmake_minimum_required(VERSION 3.14)
project(PomoCli)

# 1. Configuración de C++ (Usamos C++17 para compatibilidad con FTXUI)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Gestión de Dependencias (Descarga automática de FTXUI)
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0  # Versión estable
)

# Esto descarga y configura la librería en la carpeta 'build'
FetchContent_MakeAvailable(ftxui)

# 3. Definición del Ejecutable
# IMPORTANTE: Aquí debes listar todos tus archivos .cpp
find_package(Threads REQUIRED)

add_executable(pomodoro 
    src/main.cpp 
    src/ui.cpp
)

# 4. Enlace de Librerías
# Esto le dice al compilador dónde encontrar las funciones de FTXUI
# y añade el soporte para hilos (Threads) que necesita tu timer.
target_link_libraries(pomodoro PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
    Threads::Threads
)

