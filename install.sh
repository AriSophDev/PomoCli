#!/bin/bash

# Detener el script si ocurre algún error
set -e

echo "------------------------------------------"
echo "  🍅 Instalador de PomoCli"
echo "------------------------------------------"

# 1. Asegurarse de que estamos en la raíz del proyecto
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ Error: Ejecuta este script desde la raíz del proyecto PomoCli."
    exit 1
fi

# 2. Preparar carpeta de compilación
if [ ! -d "build" ]; then
    echo "📂 Creando carpeta de compilación..."
    mkdir build
fi

# 3. Configurar y Compilar
echo "🔨 Compilando la última versión..."
cmake -S . -B build
cmake --build build -j$(nproc)

# 4. Instalación del binario
echo "🚀 Instalando en /usr/local/bin/pomodoro..."
echo "Nota: Se requieren permisos de administrador (sudo)."
sudo cp build/pomodoro /usr/local/bin/pomodoro

# 5. Asegurar permisos de ejecución
sudo chmod +x /usr/local/bin/pomodoro

echo "------------------------------------------"
echo "✅ ¡Instalación completada con éxito!"
echo "💡 Ahora puedes usar el comando 'pomodoro' desde cualquier terminal."
echo "------------------------------------------"
