#!/usr/bin/env sh
set -e

REPO="AriSophDev/PomoCli"
VERSION="latest"

echo "📦 Instalando PomoCli..."

# Detectar OS

OS="$(uname)"
echo " Detectado: $OS"

# Verificar dependencias

need() {
command -v "$1" >/dev/null 2>&1 || {
echo " Necesitas instalar '$1' antes de continuar."
exit 1
}
}

need curl
need tar
need cmake

# URL del release (usa releases, no git clone)

if [ "$VERSION" = "latest" ]; then
URL="https://github.com/$REPO/archive/refs/heads/main.tar.gz"
else
URL="https://github.com/$REPO/archive/refs/tags/$VERSION.tar.gz"
fi

TMP_DIR="$(mktemp -d)"
cd "$TMP_DIR"

echo " Descargando código..."
curl -L "$URL" -o app.tar.gz

echo " Extrayendo..."
mkdir src
tar -xzf app.tar.gz -C src --strip-components=1

cd src

echo " Compilando..."
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

echo " Instalando..."

# Detectar si puede escribir sin sudo

PREFIX="/usr/local"
if [ ! -w "$PREFIX" ]; then
echo " Se requiere sudo para instalar en $PREFIX"
sudo cmake --install build
else
cmake --install build
fi

echo " PomoCli instalado correctamente"
echo " Ejecuta: pomocli"
